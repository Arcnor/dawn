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

#ifndef SRC_TINT_LANG_CORE_IR_INSTRUCTION_RESULT_H_
#define SRC_TINT_LANG_CORE_IR_INSTRUCTION_RESULT_H_

#include "tint/lang/core/ir/value.h"
#include "tint/utils/text/string_stream.h"

namespace tint::core::ir {

/// An instruction result in the IR.
class InstructionResult : public Castable<InstructionResult, Value> {
  public:
    /// Constructor
    /// @param type the type of the value
    explicit InstructionResult(const core::type::Type* type);

    /// Destructor
    ~InstructionResult() override;

    /// @copydoc Value::Destroy
    void Destroy() override;

    /// @returns the type of the value
    const core::type::Type* Type() override { return type_; }

    /// @copydoc Value::Clone()
    InstructionResult* Clone(CloneContext& ctx) override;

    /// Sets the type of the value to @p type
    /// @param type the new type of the value
    void SetType(const core::type::Type* type) { type_ = type; }

    /// Sets the source instruction for this value
    /// @param inst the instruction to set
    void SetSource(Instruction* inst) { source_ = inst; }

    /// @returns the source instruction, if any
    Instruction* Source() { return source_; }

  private:
    Instruction* source_ = nullptr;
    const core::type::Type* type_ = nullptr;
};

}  // namespace tint::core::ir

#endif  // SRC_TINT_LANG_CORE_IR_INSTRUCTION_RESULT_H_
