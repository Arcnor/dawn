// Copyright 2020 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SRC_AST_IF_STATEMENT_H_
#define SRC_AST_IF_STATEMENT_H_

#include <memory>
#include <utility>

#include "src/ast/block_statement.h"
#include "src/ast/else_statement.h"
#include "src/ast/expression.h"
#include "src/ast/statement.h"

namespace tint {
namespace ast {

/// An if statement
class IfStatement : public Castable<IfStatement, Statement> {
 public:
  /// Constructor
  /// @param condition the if condition
  /// @param body the if body
  IfStatement(Expression* condition, BlockStatement* body);
  /// Constructor
  /// @param source the source information
  /// @param condition the if condition
  /// @param body the if body
  IfStatement(const Source& source,
              Expression* condition,
              BlockStatement* body);
  /// Move constructor
  IfStatement(IfStatement&&);
  ~IfStatement() override;

  /// Sets the condition for the if statement
  /// @param condition the condition to set
  void set_condition(Expression* condition) { condition_ = condition; }
  /// @returns the if condition or nullptr if none set
  Expression* condition() const { return condition_; }

  /// Sets the if body
  /// @param body the if body
  void set_body(BlockStatement* body) { body_ = body; }
  /// @returns the if body
  const BlockStatement* body() const { return body_; }
  /// @returns the if body
  BlockStatement* body() { return body_; }

  /// Sets the else statements
  /// @param else_statements the else statements to set
  void set_else_statements(ElseStatementList else_statements) {
    else_statements_ = std::move(else_statements);
  }
  /// @returns the else statements
  const ElseStatementList& else_statements() const { return else_statements_; }
  /// @returns the else statements
  ElseStatementList& else_statements() { return else_statements_; }

  /// @returns true if there are else statements
  bool has_else_statements() const { return !else_statements_.empty(); }

  /// @returns true if the node is valid
  bool IsValid() const override;

  /// Writes a representation of the node to the output stream
  /// @param out the stream to write to
  /// @param indent number of spaces to indent the node when writing
  void to_str(std::ostream& out, size_t indent) const override;

 private:
  IfStatement(const IfStatement&) = delete;

  Expression* condition_ = nullptr;
  BlockStatement* body_ = nullptr;
  ElseStatementList else_statements_;
};

}  // namespace ast
}  // namespace tint

#endif  // SRC_AST_IF_STATEMENT_H_
