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

#include "tint/lang/wgsl/ast/helper_test.h"

#include "tint/utils/text/string_stream.h"

namespace tint::ast {
namespace {

using IntLiteralExpressionTest = TestHelper;

TEST_F(IntLiteralExpressionTest, SuffixNone) {
    auto* i = create<IntLiteralExpression>(42, IntLiteralExpression::Suffix::kNone);
    ASSERT_TRUE(i->Is<IntLiteralExpression>());
    EXPECT_EQ(i->value, 42);
    EXPECT_EQ(i->suffix, IntLiteralExpression::Suffix::kNone);
}

TEST_F(IntLiteralExpressionTest, SuffixI) {
    auto* i = create<IntLiteralExpression>(42, IntLiteralExpression::Suffix::kI);
    ASSERT_TRUE(i->Is<IntLiteralExpression>());
    EXPECT_EQ(i->value, 42);
    EXPECT_EQ(i->suffix, IntLiteralExpression::Suffix::kI);
}

TEST_F(IntLiteralExpressionTest, SuffixU) {
    auto* i = create<IntLiteralExpression>(42, IntLiteralExpression::Suffix::kU);
    ASSERT_TRUE(i->Is<IntLiteralExpression>());
    EXPECT_EQ(i->value, 42);
    EXPECT_EQ(i->suffix, IntLiteralExpression::Suffix::kU);
}

TEST_F(IntLiteralExpressionTest, SuffixStringStream) {
    auto to_str = [](IntLiteralExpression::Suffix suffix) {
        StringStream ss;
        ss << suffix;
        return ss.str();
    };

    EXPECT_EQ("", to_str(IntLiteralExpression::Suffix::kNone));
    EXPECT_EQ("i", to_str(IntLiteralExpression::Suffix::kI));
    EXPECT_EQ("u", to_str(IntLiteralExpression::Suffix::kU));
}

}  // namespace
}  // namespace tint::ast
