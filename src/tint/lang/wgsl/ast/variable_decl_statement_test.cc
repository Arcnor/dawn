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

#include "tint/lang/wgsl/ast/variable_decl_statement.h"

#include "gtest/gtest-spi.h"
#include "tint/lang/wgsl/ast/helper_test.h"

namespace tint::ast {
namespace {

using VariableDeclStatementTest = TestHelper;

TEST_F(VariableDeclStatementTest, Creation) {
    auto* var = Var("a", ty.f32());

    auto* stmt = create<VariableDeclStatement>(var);
    EXPECT_EQ(stmt->variable, var);
}

TEST_F(VariableDeclStatementTest, Creation_WithSource) {
    auto* var = Var("a", ty.f32());

    auto* stmt = create<VariableDeclStatement>(Source{Source::Location{20, 2}}, var);
    auto src = stmt->source;
    EXPECT_EQ(src.range.begin.line, 20u);
    EXPECT_EQ(src.range.begin.column, 2u);
}

TEST_F(VariableDeclStatementTest, IsVariableDecl) {
    auto* var = Var("a", ty.f32());

    auto* stmt = create<VariableDeclStatement>(var);
    EXPECT_TRUE(stmt->Is<VariableDeclStatement>());
}

TEST_F(VariableDeclStatementTest, Assert_Null_Variable) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b;
            b.create<VariableDeclStatement>(nullptr);
        },
        "internal compiler error");
}

TEST_F(VariableDeclStatementTest, Assert_DifferentGenerationID_Variable) {
    EXPECT_FATAL_FAILURE(
        {
            ProgramBuilder b1;
            ProgramBuilder b2;
            b1.create<VariableDeclStatement>(b2.Var("a", b2.ty.f32()));
        },
        "internal compiler error");
}

}  // namespace
}  // namespace tint::ast
