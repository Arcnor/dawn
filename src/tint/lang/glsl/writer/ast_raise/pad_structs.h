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

#ifndef SRC_TINT_LANG_GLSL_WRITER_AST_RAISE_PAD_STRUCTS_H_
#define SRC_TINT_LANG_GLSL_WRITER_AST_RAISE_PAD_STRUCTS_H_

#include "tint/lang/wgsl/ast/transform/transform.h"

namespace tint::glsl::writer {

/// This transform turns all explicit alignment and sizing into padding
/// members of structs. This is required for GLSL ES, since it not support
/// the offset= decoration.
///
/// @note This transform requires the CanonicalizeEntryPointIO transform to have been run first.
class PadStructs final : public Castable<PadStructs, ast::transform::Transform> {
  public:
    /// Constructor
    PadStructs();

    /// Destructor
    ~PadStructs() override;

    /// @copydoc ast::transform::Transform::Apply
    ApplyResult Apply(const Program& program,
                      const ast::transform::DataMap& inputs,
                      ast::transform::DataMap& outputs) const override;
};

}  // namespace tint::glsl::writer

#endif  // SRC_TINT_LANG_GLSL_WRITER_AST_RAISE_PAD_STRUCTS_H_
