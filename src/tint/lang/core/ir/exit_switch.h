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

#ifndef SRC_TINT_LANG_CORE_IR_EXIT_SWITCH_H_
#define SRC_TINT_LANG_CORE_IR_EXIT_SWITCH_H_

#include <string>

#include "tint/lang/core/ir/exit.h"
#include "tint/utils/rtti/castable.h"

// Forward declarations
namespace tint::core::ir {
class Switch;
}  // namespace tint::core::ir

namespace tint::core::ir {

/// A exit switch instruction.
class ExitSwitch final : public Castable<ExitSwitch, Exit> {
  public:
    /// The base offset in Operands() for the args
    static constexpr size_t kArgsOperandOffset = 0;

    /// Constructor
    /// @param sw the switch being exited
    /// @param args the target MultiInBlock arguments
    explicit ExitSwitch(ir::Switch* sw, VectorRef<Value*> args = tint::Empty);
    ~ExitSwitch() override;

    /// @copydoc Instruction::Clone()
    ExitSwitch* Clone(CloneContext& ctx) override;

    /// Re-associates the exit with the given switch instruction
    /// @param s the new switch to exit from
    void SetSwitch(ir::Switch* s);

    /// @returns the switch being exited
    ir::Switch* Switch();

    /// @returns the friendly name for the instruction
    std::string FriendlyName() override { return "exit_switch"; }
};

}  // namespace tint::core::ir

#endif  // SRC_TINT_LANG_CORE_IR_EXIT_SWITCH_H_
