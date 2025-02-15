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

#ifndef SRC_TINT_LANG_WGSL_AST_NODE_H_
#define SRC_TINT_LANG_WGSL_AST_NODE_H_

#include <string>

#include "tint/lang/wgsl/ast/node_id.h"
#include "tint/utils/diagnostic/source.h"
#include "tint/utils/id/generation_id.h"
#include "tint/utils/rtti/castable.h"

// Forward declarations
namespace tint::ast {
class CloneContext;
}

namespace tint::ast {

/// AST base class node
class Node : public Castable<Node> {
  public:
    ~Node() override;

    /// Performs a deep clone of this object using the CloneContext `ctx`.
    /// @param ctx the clone context
    /// @return the newly cloned object
    virtual const Node* Clone(CloneContext& ctx) const = 0;

    /// The identifier of the program that owns this node
    const GenerationID generation_id;

    /// The node identifier, unique for the program.
    const NodeID node_id;

    /// The node source data
    const Source source;

  protected:
    /// Create a new node
    /// @param pid the identifier of the program that owns this node
    /// @param nid the unique node identifier
    /// @param src the input source for the node
    Node(GenerationID pid, NodeID nid, const Source& src);

  private:
    Node(const Node&) = delete;
    Node(Node&&) = delete;
};

}  // namespace tint::ast

namespace tint {

/// @param node a pointer to an AST node
/// @returns the GenerationID of the given AST node.
inline GenerationID GenerationIDOf(const ast::Node* node) {
    return node ? node->generation_id : GenerationID();
}

}  // namespace tint

#endif  // SRC_TINT_LANG_WGSL_AST_NODE_H_
