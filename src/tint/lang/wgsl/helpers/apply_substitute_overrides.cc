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

#include "tint/lang/wgsl/helpers/apply_substitute_overrides.h"

#include <memory>
#include <utility>

#include "tint/lang/wgsl/ast/transform/manager.h"
#include "tint/lang/wgsl/ast/transform/substitute_override.h"
#include "tint/lang/wgsl/inspector/inspector.h"
#include "tint/lang/wgsl/program/program.h"

namespace tint::wgsl {

std::optional<Program> ApplySubstituteOverrides(const Program& program) {
    ast::transform::SubstituteOverride::Config cfg;
    inspector::Inspector inspector(program);
    auto default_values = inspector.GetOverrideDefaultValues();
    for (const auto& [override_id, scalar] : default_values) {
        // If the override is not null, then it has a default value, we can just let it use the
        // provided default instead of overriding.
        if (scalar.IsNull()) {
            cfg.map.insert({override_id, 0.0});
        }
    }

    if (default_values.empty()) {
        return std::nullopt;
    }

    ast::transform::DataMap override_data;
    override_data.Add<ast::transform::SubstituteOverride::Config>(cfg);

    ast::transform::Manager mgr;
    mgr.append(std::make_unique<ast::transform::SubstituteOverride>());

    ast::transform::DataMap outputs;
    return mgr.Run(program, override_data, outputs);
}

}  // namespace tint::wgsl
