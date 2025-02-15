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

#include "gtest/gtest-spi.h"
#include "tint/lang/wgsl/ast/helper_test.h"

namespace tint::ast {
namespace {

using MemberAccessorExpressionTest = TestHelper;

TEST_F(MemberAccessorExpressionTest, Creation) {
    auto* str = Expr("structure");
    auto* mem = Ident("member");

    auto* stmt = create<MemberAccessorExpression>(str, mem);
    EXPECT_EQ(stmt->object, str);
    EXPECT_EQ(stmt->member, mem);
}

TEST_F(MemberAccessorExpressionTest, Creation_WithSource) {
    auto* stmt = create<MemberAccessorExpression>(Source{Source::Location{20, 2}},
                                                  Expr("structure"), Ident("member"));
    auto src = stmt->source;
    EXPECT_EQ(src.range.begin.line, 20u);
    EXPECT_EQ(src.range.begin.column, 2u);
}

TEST_F(MemberAccessorExpressionTest, IsMemberAccessor) {
    auto* stmt = create<MemberAccessorExpression>(Expr("structure"), Ident("member"));
    EXPECT_TRUE(stmt->Is<MemberAccessorExpression>());
}

TEST_F(MemberAccessorExpressionTest, Assert_Null_Struct) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b;
            b.create<MemberAccessorExpression>(nullptr, b.Ident("member"));
        },
        "internal compiler error");
}

TEST_F(MemberAccessorExpressionTest, Assert_Null_Member) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b;
            b.create<MemberAccessorExpression>(b.Expr("struct"), nullptr);
        },
        "internal compiler error");
}

TEST_F(MemberAccessorExpressionTest, Assert_DifferentGenerationID_Struct) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b1;
            ProgramBuilder b2;
            b1.create<MemberAccessorExpression>(b2.Expr("structure"), b1.Ident("member"));
        },
        "internal compiler error");
}

TEST_F(MemberAccessorExpressionTest, Assert_DifferentGenerationID_Member) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b1;
            ProgramBuilder b2;
            b1.create<MemberAccessorExpression>(b1.Expr("structure"), b2.Ident("member"));
        },
        "internal compiler error");
}

TEST_F(MemberAccessorExpressionTest, Assert_MemberNotTemplated) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b;
            b.create<MemberAccessorExpression>(b.Expr("structure"),
                                               b.Ident("member", "a", "b", "c"));
        },
        "internal compiler error");
}

}  // namespace
}  // namespace tint::ast
