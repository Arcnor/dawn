// Copyright 2021 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/tint/transform/binding_remapper.h"

#include <string>
#include <unordered_set>
#include <utility>

#include "src/tint/ast/disable_validation_attribute.h"
#include "src/tint/program_builder.h"
#include "src/tint/sem/function.h"
#include "src/tint/sem/variable.h"
#include "src/tint/utils/string.h"

TINT_INSTANTIATE_TYPEINFO(tint::transform::BindingRemapper);
TINT_INSTANTIATE_TYPEINFO(tint::transform::BindingRemapper::Remappings);

namespace tint::transform {

BindingRemapper::Remappings::Remappings(BindingPoints bp, AccessControls ac, bool may_collide)
    : binding_points(std::move(bp)),
      access_controls(std::move(ac)),
      allow_collisions(may_collide) {}

BindingRemapper::Remappings::Remappings(const Remappings&) = default;
BindingRemapper::Remappings::~Remappings() = default;

BindingRemapper::BindingRemapper() = default;
BindingRemapper::~BindingRemapper() = default;

Transform::ApplyResult BindingRemapper::Apply(const Program* src,
                                              const DataMap& inputs,
                                              DataMap&) const {
    ProgramBuilder b;
    CloneContext ctx{&b, src, /* auto_clone_symbols */ true};

    auto* remappings = inputs.Get<Remappings>();
    if (!remappings) {
        b.Diagnostics().add_error(diag::System::Transform,
                                  "missing transform data for " + std::string(TypeInfo().name));
        return Program(std::move(b));
    }

    if (remappings->binding_points.empty() && remappings->access_controls.empty()) {
        return SkipTransform;
    }

    // A set of post-remapped binding points that need to be decorated with a
    // DisableValidationAttribute to disable binding-point-collision validation
    std::unordered_set<sem::BindingPoint> add_collision_attr;

    if (remappings->allow_collisions) {
        // Scan for binding point collisions generated by this transform.
        // Populate all collisions in the `add_collision_attr` set.
        for (auto* func_ast : src->AST().Functions()) {
            if (!func_ast->IsEntryPoint()) {
                continue;
            }
            auto* func = src->Sem().Get(func_ast);
            std::unordered_map<sem::BindingPoint, int> binding_point_counts;
            for (auto* global : func->TransitivelyReferencedGlobals()) {
                if (global->Declaration()->HasBindingPoint()) {
                    BindingPoint from = global->BindingPoint();

                    auto bp_it = remappings->binding_points.find(from);
                    if (bp_it != remappings->binding_points.end()) {
                        // Remapped
                        BindingPoint to = bp_it->second;
                        if (binding_point_counts[to]++) {
                            add_collision_attr.emplace(to);
                        }
                    } else {
                        // No remapping
                        if (binding_point_counts[from]++) {
                            add_collision_attr.emplace(from);
                        }
                    }
                }
            }
        }
    }

    for (auto* var : src->AST().Globals<ast::Var>()) {
        if (var->HasBindingPoint()) {
            auto* global_sem = src->Sem().Get<sem::GlobalVariable>(var);

            // The original binding point
            BindingPoint from = global_sem->BindingPoint();

            // The binding point after remapping
            BindingPoint bp = from;

            // Replace any group or binding attributes.
            // Note: This has to be performed *before* remapping access controls, as
            // `ctx.Clone(var->attributes)` depend on these replacements.
            auto bp_it = remappings->binding_points.find(from);
            if (bp_it != remappings->binding_points.end()) {
                BindingPoint to = bp_it->second;
                auto* new_group = b.Group(AInt(to.group));
                auto* new_binding = b.Binding(AInt(to.binding));

                auto* old_group = ast::GetAttribute<ast::GroupAttribute>(var->attributes);
                auto* old_binding = ast::GetAttribute<ast::BindingAttribute>(var->attributes);

                ctx.Replace(old_group, new_group);
                ctx.Replace(old_binding, new_binding);
                bp = to;
            }

            // Replace any access controls.
            auto ac_it = remappings->access_controls.find(from);
            if (ac_it != remappings->access_controls.end()) {
                builtin::Access access = ac_it->second;
                if (access == builtin::Access::kUndefined) {
                    b.Diagnostics().add_error(diag::System::Transform,
                                              "invalid access mode (" +
                                                  std::to_string(static_cast<uint32_t>(access)) +
                                                  ")");
                    return Program(std::move(b));
                }
                auto* sem = src->Sem().Get(var);
                if (sem->AddressSpace() != type::AddressSpace::kStorage) {
                    b.Diagnostics().add_error(
                        diag::System::Transform,
                        "cannot apply access control to variable with address space " +
                            std::string(utils::ToString(sem->AddressSpace())));
                    return Program(std::move(b));
                }
                auto* ty = sem->Type()->UnwrapRef();
                auto inner_ty = CreateASTTypeFor(ctx, ty);
                auto* new_var =
                    b.create<ast::Var>(ctx.Clone(var->source),                  // source
                                       b.Ident(ctx.Clone(var->name->symbol)),   // name
                                       inner_ty,                                // type
                                       ctx.Clone(var->declared_address_space),  // address space
                                       b.Expr(access),                          // access
                                       ctx.Clone(var->initializer),             // initializer
                                       ctx.Clone(var->attributes));             // attributes
                ctx.Replace(var, new_var);
            }

            // Add `DisableValidationAttribute`s if required
            if (add_collision_attr.count(bp)) {
                auto* attribute = b.Disable(ast::DisabledValidation::kBindingPointCollision);
                ctx.InsertBefore(var->attributes, *var->attributes.begin(), attribute);
            }
        }
    }

    ctx.Clone();
    return Program(std::move(b));
}

}  // namespace tint::transform
