// Copyright 2020 The Dawn & Tint Authors
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

#ifndef SRC_TINT_LANG_WGSL_AST_BLOCK_STATEMENT_H_
#define SRC_TINT_LANG_WGSL_AST_BLOCK_STATEMENT_H_

#include <utility>

#include "tint/lang/wgsl/ast/statement.h"
#include "tint/utils/containers/vector.h"
#include "tint/utils/diagnostic/source.h"

// Forward declarations
namespace tint::ast {
class Attribute;
}  // namespace tint::ast

namespace tint::ast {

/// A block statement
class BlockStatement final : public Castable<BlockStatement, Statement> {
  public:
    /// Constructor
    /// @param pid the identifier of the program that owns this node
    /// @param nid the unique node identifier
    /// @param source the block statement source
    /// @param statements the statements
    /// @param attributes the block statement attributes
    BlockStatement(GenerationID pid,
                   NodeID nid,
                   const Source& source,
                   VectorRef<const Statement*> statements,
                   VectorRef<const Attribute*> attributes);

    /// Destructor
    ~BlockStatement() override;

    /// @returns true if the block has no statements
    bool Empty() const { return statements.IsEmpty(); }

    /// @returns the last statement in the block or nullptr if block empty
    const Statement* Last() const { return statements.IsEmpty() ? nullptr : statements.Back(); }

    /// Clones this node and all transitive child nodes using the `CloneContext`
    /// `ctx`.
    /// @param ctx the clone context
    /// @return the newly cloned node
    const BlockStatement* Clone(CloneContext& ctx) const override;

    /// the statement list
    const tint::Vector<const Statement*, 8> statements;

    /// the attribute list
    const tint::Vector<const Attribute*, 4> attributes;
};

}  // namespace tint::ast

#endif  // SRC_TINT_LANG_WGSL_AST_BLOCK_STATEMENT_H_
