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

#ifndef SRC_TINT_LANG_CORE_IR_USER_CALL_H_
#define SRC_TINT_LANG_CORE_IR_USER_CALL_H_

#include <string>

#include "tint/lang/core/ir/call.h"
#include "tint/lang/core/ir/function.h"
#include "tint/utils/rtti/castable.h"

namespace tint::core::ir {

/// A user call instruction in the IR.
class UserCall final : public Castable<UserCall, Call> {
  public:
    /// The offset in Operands() for the function being called
    static constexpr size_t kFunctionOperandOffset = 0;

    /// The base offset in Operands() for the call arguments
    static constexpr size_t kArgsOperandOffset = 1;

    /// Constructor
    /// @param result the result value
    /// @param func the function being called
    /// @param args the function arguments
    UserCall(InstructionResult* result, Function* func, VectorRef<Value*> args);
    ~UserCall() override;

    /// @copydoc Instruction::Clone()
    UserCall* Clone(CloneContext& ctx) override;

    /// @returns the call arguments
    tint::Slice<Value*> Args() override { return operands_.Slice().Offset(kArgsOperandOffset); }

    /// Replaces the call arguments to @p arguments
    /// @param arguments the new call arguments
    void SetArgs(VectorRef<Value*> arguments);

    /// @returns the called function
    Function* Target() { return operands_[kFunctionOperandOffset]->As<ir::Function>(); }

    /// Sets called function
    /// @param target the new target of the call
    void SetTarget(Function* target) { SetOperand(kFunctionOperandOffset, target); }

    /// @returns the friendly name for the instruction
    std::string FriendlyName() override { return "call"; }
};

}  // namespace tint::core::ir

#endif  // SRC_TINT_LANG_CORE_IR_USER_CALL_H_
