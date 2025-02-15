// Copyright 2021 The Dawn & Tint Authors
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

#ifndef SRC_TINT_LANG_WGSL_SEM_LOOP_STATEMENT_H_
#define SRC_TINT_LANG_WGSL_SEM_LOOP_STATEMENT_H_

#include "tint/lang/wgsl/sem/block_statement.h"

// Forward declarations
namespace tint::ast {
class LoopStatement;
}  // namespace tint::ast

namespace tint::sem {

/// Holds semantic information about a loop statement
class LoopStatement final : public Castable<LoopStatement, CompoundStatement> {
  public:
    /// Constructor
    /// @param declaration the AST node for this loop statement
    /// @param parent the owning statement
    /// @param function the owning function
    LoopStatement(const ast::LoopStatement* declaration,
                  const CompoundStatement* parent,
                  const sem::Function* function);

    /// Destructor
    ~LoopStatement() override;
};

/// Holds semantic information about a loop continuing block
class LoopContinuingBlockStatement final
    : public Castable<LoopContinuingBlockStatement, BlockStatement> {
  public:
    /// Constructor
    /// @param declaration the AST node for this block statement
    /// @param parent the owning statement
    /// @param function the owning function
    LoopContinuingBlockStatement(const ast::BlockStatement* declaration,
                                 const CompoundStatement* parent,
                                 const sem::Function* function);

    /// Destructor
    ~LoopContinuingBlockStatement() override;

    /// @returns the AST node
    const ast::BlockStatement* Declaration() const;
};

}  // namespace tint::sem

#endif  // SRC_TINT_LANG_WGSL_SEM_LOOP_STATEMENT_H_
