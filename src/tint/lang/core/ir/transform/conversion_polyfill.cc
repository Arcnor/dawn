// Copyright 2023 The Dawn & Tint Authors
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "tint/lang/core/ir/transform/conversion_polyfill.h"

#include <cmath>
#include <utility>

#include "tint/lang/core/ir/builder.h"
#include "tint/lang/core/ir/module.h"
#include "tint/lang/core/ir/validator.h"

using namespace tint::core::fluent_types;     // NOLINT
using namespace tint::core::number_suffixes;  // NOLINT

namespace tint::core::ir::transform {

namespace {

/// PIMPL state for the transform.
struct State {
    /// The polyfill config.
    const ConversionPolyfillConfig& config;

    /// The IR module.
    Module& ir;

    /// The IR builder.
    Builder b{ir};

    /// The type manager.
    core::type::Manager& ty{ir.Types()};

    /// The symbol table.
    SymbolTable& sym{ir.symbols};

    /// Map from integer type to its f32toi helper function.
    Hashmap<const type::Type*, Function*, 4> f32toi_helpers{};

    /// Map from integer type to its f16toi helper function.
    Hashmap<const type::Type*, Function*, 4> f16toi_helpers{};

    /// Process the module.
    void Process() {
        // Find the conversion instructions that need to be polyfilled.
        Vector<ir::Convert*, 64> ftoi_worklist;
        for (auto* inst : ir.instructions.Objects()) {
            if (!inst->Alive()) {
                continue;
            }
            if (auto* convert = inst->As<ir::Convert>()) {
                auto* src_ty = convert->Args()[0]->Type();
                auto* res_ty = convert->Result()->Type();
                if (config.ftoi &&                          //
                    src_ty->is_float_scalar_or_vector() &&  //
                    res_ty->is_integer_scalar_or_vector()) {
                    ftoi_worklist.Push(convert);
                }
            }
        }

        // Polyfill the conversion instructions that we found.
        for (auto* convert : ftoi_worklist) {
            auto* replacement = ftoi(convert);

            // Replace the old conversion instruction result with the new value.
            if (auto name = ir.NameOf(convert->Result())) {
                ir.SetName(replacement, name);
            }
            convert->Result()->ReplaceAllUsesWith(replacement);
            convert->Destroy();
        }
    }

    /// Replace a conversion instruction with a call to helper function that manually clamps the
    /// result to within the limit of the destination type.
    /// @param convert the conversion instruction
    /// @returns the replacement value
    ir::Value* ftoi(ir::Convert* convert) {
        auto* res_ty = convert->Result()->Type();
        auto* src_ty = convert->Args()[0]->Type();
        auto* src_el_ty = src_ty->DeepestElement();

        auto& helpers = src_el_ty->Is<type::F32>() ? f32toi_helpers : f16toi_helpers;
        auto* helper = helpers.GetOrCreate(res_ty, [&] {
            // Generate a name for the helper function.
            StringStream name;
            name << "tint_";
            if (auto* src_vec = src_ty->As<type::Vector>()) {
                name << "v" << src_vec->Width() << src_vec->type()->FriendlyName();
            } else {
                name << src_ty->FriendlyName();
            }
            name << "_to_";
            if (auto* res_vec = res_ty->As<type::Vector>()) {
                name << "v" << res_vec->Width() << res_vec->type()->FriendlyName();
            } else {
                name << res_ty->FriendlyName();
            }

            // Generate constants for the limits.
            struct {
                ir::Constant* low_limit_f = nullptr;
                ir::Constant* high_limit_f = nullptr;
                ir::Constant* low_limit_i = nullptr;
                ir::Constant* high_limit_i = nullptr;
            } limits;

            // Integer limits.
            if (res_ty->is_signed_integer_scalar_or_vector()) {
                limits.low_limit_i = MatchWidth(b.Constant(i32(INT32_MIN)), res_ty);
                limits.high_limit_i = MatchWidth(b.Constant(i32(INT32_MAX)), res_ty);
            } else {
                limits.low_limit_i = MatchWidth(b.Constant(u32(0)), res_ty);
                limits.high_limit_i = MatchWidth(b.Constant(u32(UINT32_MAX)), res_ty);
            }

            // Largest integers representable in the source floating point format.
            if (src_el_ty->Is<type::F32>()) {
                if (res_ty->is_signed_integer_scalar_or_vector()) {
                    // INT32_MIN is -(2^31), which is exactly representable as an f32.
                    // INT32_MAX is (2^31 - 1), which is not exactly representable as an f32, so we
                    // instead use the next highest integer value in the f32 domain.
                    const float kMaxI32AsF32 = std::nexttowardf(0x1p+31f, 0.0L);
                    limits.low_limit_f = MatchWidth(b.Constant(f32(INT32_MIN)), res_ty);
                    limits.high_limit_f = MatchWidth(b.Constant(f32(kMaxI32AsF32)), res_ty);
                } else {
                    // UINT32_MAX is (2^32 - 1), which is not exactly representable as an f32, so we
                    // instead use the next highest integer value in the f32 domain.
                    const float kMaxU32AsF32 = std::nexttowardf(0x1p+32f, 0.0L);
                    limits.low_limit_f = MatchWidth(b.Constant(f32(0)), res_ty);
                    limits.high_limit_f = MatchWidth(b.Constant(f32(kMaxU32AsF32)), res_ty);
                }
            } else if (src_el_ty->Is<type::F16>()) {
                constexpr float MAX_F16 = 65504;
                if (res_ty->is_signed_integer_scalar_or_vector()) {
                    limits.low_limit_f = MatchWidth(b.Constant(f16(-MAX_F16)), res_ty);
                    limits.high_limit_f = MatchWidth(b.Constant(f16(MAX_F16)), res_ty);
                } else {
                    limits.low_limit_f = MatchWidth(b.Constant(f16(0)), res_ty);
                    limits.high_limit_f = MatchWidth(b.Constant(f16(MAX_F16)), res_ty);
                }
            } else {
                TINT_UNIMPLEMENTED() << "unhandled floating-point type";
            }

            // Create the helper function.
            auto* func = b.Function(name.str(), res_ty);
            auto* value = b.FunctionParam("value", src_ty);
            func->SetParams({value});
            b.Append(func->Block(), [&] {
                auto* bool_ty = MatchWidth(ty.bool_(), res_ty);

                auto* converted = b.Convert(res_ty, value);

                // low = select(low_limit_i, i32(value), value >= low_limit_f)
                auto* low_cond = b.GreaterThanEqual(bool_ty, value, limits.low_limit_f);
                auto* select_low = b.Call(res_ty, core::BuiltinFn::kSelect, limits.low_limit_i,
                                          converted, low_cond);

                // result = select(high_limit_i, low, value <= high_limit_f)
                auto* high_cond = b.LessThanEqual(bool_ty, value, limits.high_limit_f);
                auto* select_high = b.Call(res_ty, core::BuiltinFn::kSelect, limits.high_limit_i,
                                           select_low, high_cond);

                b.Return(func, select_high->Result());
            });
            return func;
        });

        // Call the helper function, splatting the arguments to match the target vector width.
        auto* call = b.Call(res_ty, helper, convert->Args()[0]);
        call->InsertBefore(convert);
        return call->Result();
    }

    /// Return a type with element type @p type that has the same number of vector components as
    /// @p match. If @p match is scalar just return @p type.
    /// @param el_ty the type to extend
    /// @param match the type to match the component count of
    /// @returns a type with the same number of vector components as @p match
    const core::type::Type* MatchWidth(const core::type::Type* el_ty,
                                       const core::type::Type* match) {
        if (auto* vec = match->As<core::type::Vector>()) {
            return ty.vec(el_ty, vec->Width());
        }
        return el_ty;
    }

    /// Return a constant that has the same number of vector components as @p match, each with the
    /// value @p element. If @p match is scalar just return @p element.
    /// @param element the value to extend
    /// @param match the type to match the component count of
    /// @returns a value with the same number of vector components as @p match
    ir::Constant* MatchWidth(ir::Constant* element, const core::type::Type* match) {
        if (auto* vec = match->As<core::type::Vector>()) {
            return b.Splat(MatchWidth(element->Type(), match), element, vec->Width());
        }
        return element;
    }
};

}  // namespace

Result<SuccessType> ConversionPolyfill(Module& ir, const ConversionPolyfillConfig& config) {
    auto result = ValidateAndDumpIfNeeded(ir, "ConversionPolyfill transform");
    if (!result) {
        return result;
    }

    State{config, ir}.Process();

    return Success;
}

}  // namespace tint::core::ir::transform
