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

#include "tint/lang/core/ir/instruction_result.h"

#include "tint/lang/core/ir/clone_context.h"
#include "tint/lang/core/ir/constant.h"
#include "tint/lang/core/ir/instruction.h"
#include "tint/lang/core/ir/module.h"
#include "tint/utils/ice/ice.h"

TINT_INSTANTIATE_TYPEINFO(tint::core::ir::InstructionResult);

namespace tint::core::ir {

InstructionResult::InstructionResult(const core::type::Type* type) : type_(type) {
    TINT_ASSERT(type_ != nullptr);
}

InstructionResult::~InstructionResult() = default;

void InstructionResult::Destroy() {
    TINT_ASSERT(source_ == nullptr);
    Base::Destroy();
}

InstructionResult* InstructionResult::Clone(CloneContext& ctx) {
    // Do not clone the `Source`. It will be set when this result is placed in the new parent
    // instruction.
    return ctx.ir.values.Create<InstructionResult>(type_);
}

}  // namespace tint::core::ir
