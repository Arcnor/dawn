// Copyright 2023 The Dawn & Tint Authors
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

#include "gmock/gmock.h"
#include "gtest/gtest-spi.h"
#include "tint/lang/core/fluent_types.h"
#include "tint/lang/core/ir/builder.h"
#include "tint/lang/core/ir/instruction.h"
#include "tint/lang/core/ir/ir_helper_test.h"

using namespace tint::core::number_suffixes;  // NOLINT
using namespace tint::core::fluent_types;     // NOLINT

namespace tint::core::ir {
namespace {

using IR_UnaryTest = IRTestHelper;

TEST_F(IR_UnaryTest, CreateComplement) {
    auto* inst = b.Complement(mod.Types().i32(), 4_i);

    ASSERT_TRUE(inst->Is<Unary>());
    EXPECT_EQ(inst->Op(), UnaryOp::kComplement);

    ASSERT_TRUE(inst->Val()->Is<Constant>());
    auto lhs = inst->Val()->As<Constant>()->Value();
    ASSERT_TRUE(lhs->Is<core::constant::Scalar<i32>>());
    EXPECT_EQ(4_i, lhs->As<core::constant::Scalar<i32>>()->ValueAs<i32>());
}

TEST_F(IR_UnaryTest, CreateNegation) {
    auto* inst = b.Negation(mod.Types().i32(), 4_i);

    ASSERT_TRUE(inst->Is<Unary>());
    EXPECT_EQ(inst->Op(), UnaryOp::kNegation);

    ASSERT_TRUE(inst->Val()->Is<Constant>());
    auto lhs = inst->Val()->As<Constant>()->Value();
    ASSERT_TRUE(lhs->Is<core::constant::Scalar<i32>>());
    EXPECT_EQ(4_i, lhs->As<core::constant::Scalar<i32>>()->ValueAs<i32>());
}

TEST_F(IR_UnaryTest, Usage) {
    auto* inst = b.Negation(mod.Types().i32(), 4_i);

    EXPECT_EQ(inst->Op(), UnaryOp::kNegation);

    ASSERT_NE(inst->Val(), nullptr);
    EXPECT_THAT(inst->Val()->Usages(), testing::UnorderedElementsAre(Usage{inst, 0u}));
}

TEST_F(IR_UnaryTest, Result) {
    auto* inst = b.Negation(mod.Types().i32(), 4_i);
    EXPECT_TRUE(inst->HasResults());
    EXPECT_FALSE(inst->HasMultiResults());
    EXPECT_TRUE(inst->Result()->Is<InstructionResult>());
    EXPECT_EQ(inst->Result()->Source(), inst);
}

TEST_F(IR_UnaryTest, Fail_NullType) {
    EXPECT_FATAL_FAILURE(
        {
            Module mod;
            Builder b{mod};
            b.Negation(nullptr, 1_i);
        },
        "");
}

TEST_F(IR_UnaryTest, Clone) {
    auto* inst = b.Complement(mod.Types().i32(), 4_i);
    auto* new_inst = clone_ctx.Clone(inst);

    EXPECT_NE(inst, new_inst);
    EXPECT_NE(nullptr, new_inst->Result());
    EXPECT_NE(inst->Result(), new_inst->Result());

    EXPECT_EQ(UnaryOp::kComplement, new_inst->Op());

    auto new_val = new_inst->Val()->As<Constant>()->Value();
    ASSERT_TRUE(new_val->Is<core::constant::Scalar<i32>>());
    EXPECT_EQ(4_i, new_val->As<core::constant::Scalar<i32>>()->ValueAs<i32>());
}

}  // namespace
}  // namespace tint::core::ir
