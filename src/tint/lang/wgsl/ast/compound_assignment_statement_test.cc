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

#include "tint/lang/wgsl/ast/compound_assignment_statement.h"

#include "gtest/gtest-spi.h"
#include "tint/lang/wgsl/ast/helper_test.h"

using namespace tint::core::number_suffixes;  // NOLINT

namespace tint::ast {
namespace {

using CompoundAssignmentStatementTest = TestHelper;

TEST_F(CompoundAssignmentStatementTest, Creation) {
    auto* lhs = Expr("lhs");
    auto* rhs = Expr("rhs");
    auto op = core::BinaryOp::kAdd;

    auto* stmt = create<CompoundAssignmentStatement>(lhs, rhs, op);
    EXPECT_EQ(stmt->lhs, lhs);
    EXPECT_EQ(stmt->rhs, rhs);
    EXPECT_EQ(stmt->op, op);
}

TEST_F(CompoundAssignmentStatementTest, CreationWithSource) {
    auto* lhs = Expr("lhs");
    auto* rhs = Expr("rhs");
    auto op = core::BinaryOp::kMultiply;

    auto* stmt = create<CompoundAssignmentStatement>(Source{Source::Location{20, 2}}, lhs, rhs, op);
    auto src = stmt->source;
    EXPECT_EQ(src.range.begin.line, 20u);
    EXPECT_EQ(src.range.begin.column, 2u);
}

TEST_F(CompoundAssignmentStatementTest, IsCompoundAssign) {
    auto* lhs = Expr("lhs");
    auto* rhs = Expr("rhs");
    auto op = core::BinaryOp::kSubtract;

    auto* stmt = create<CompoundAssignmentStatement>(lhs, rhs, op);
    EXPECT_TRUE(stmt->Is<CompoundAssignmentStatement>());
}

TEST_F(CompoundAssignmentStatementTest, Assert_Null_LHS) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b;
            b.create<CompoundAssignmentStatement>(nullptr, b.Expr(1_i), core::BinaryOp::kAdd);
        },
        "internal compiler error");
}

TEST_F(CompoundAssignmentStatementTest, Assert_Null_RHS) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b;
            b.create<CompoundAssignmentStatement>(b.Expr(1_i), nullptr, core::BinaryOp::kAdd);
        },
        "internal compiler error");
}

TEST_F(CompoundAssignmentStatementTest, Assert_DifferentGenerationID_LHS) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b1;
            ProgramBuilder b2;
            b1.create<CompoundAssignmentStatement>(b2.Expr("lhs"), b1.Expr("rhs"),
                                                   core::BinaryOp::kAdd);
        },
        "internal compiler error");
}

TEST_F(CompoundAssignmentStatementTest, Assert_DifferentGenerationID_RHS) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b1;
            ProgramBuilder b2;
            b1.create<CompoundAssignmentStatement>(b1.Expr("lhs"), b2.Expr("rhs"),
                                                   core::BinaryOp::kAdd);
        },
        "internal compiler error");
}

}  // namespace
}  // namespace tint::ast
