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

#include "tint/lang/core/ir/load_vector_element.h"

#include "tint/lang/core/ir/clone_context.h"
#include "tint/lang/core/ir/module.h"

TINT_INSTANTIATE_TYPEINFO(tint::core::ir::LoadVectorElement);

namespace tint::core::ir {

LoadVectorElement::LoadVectorElement(InstructionResult* result, ir::Value* from, ir::Value* index) {
    flags_.Add(Flag::kSequenced);

    AddOperand(LoadVectorElement::kFromOperandOffset, from);
    AddOperand(LoadVectorElement::kIndexOperandOffset, index);
    AddResult(result);
}

LoadVectorElement::~LoadVectorElement() = default;

LoadVectorElement* LoadVectorElement::Clone(CloneContext& ctx) {
    auto* new_result = ctx.Clone(Result());
    auto* from = ctx.Remap(From());
    auto* index = ctx.Remap(Index());
    return ctx.ir.instructions.Create<LoadVectorElement>(new_result, from, index);
}

}  // namespace tint::core::ir
