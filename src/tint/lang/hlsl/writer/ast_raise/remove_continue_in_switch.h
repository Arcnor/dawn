// Copyright 2022 The Dawn & Tint Authors
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

#ifndef SRC_TINT_LANG_HLSL_WRITER_AST_RAISE_REMOVE_CONTINUE_IN_SWITCH_H_
#define SRC_TINT_LANG_HLSL_WRITER_AST_RAISE_REMOVE_CONTINUE_IN_SWITCH_H_

#include "tint/lang/wgsl/ast/transform/transform.h"

namespace tint::hlsl::writer {

/// This transform replaces continue statements in switch cases with setting a
/// bool variable, and checking if the variable is set after the switch to
/// continue. It is necessary to work around FXC "error X3708: continue cannot
/// be used in a switch". See crbug.com/tint/1080.
class RemoveContinueInSwitch final
    : public Castable<RemoveContinueInSwitch, ast::transform::Transform> {
  public:
    /// Constructor
    RemoveContinueInSwitch();

    /// Destructor
    ~RemoveContinueInSwitch() override;

    /// @copydoc ast::transform::Transform::Apply
    ApplyResult Apply(const Program& program,
                      const ast::transform::DataMap& inputs,
                      ast::transform::DataMap& outputs) const override;

  private:
    struct State;
};

}  // namespace tint::hlsl::writer

#endif  // SRC_TINT_LANG_HLSL_WRITER_AST_RAISE_REMOVE_CONTINUE_IN_SWITCH_H_
