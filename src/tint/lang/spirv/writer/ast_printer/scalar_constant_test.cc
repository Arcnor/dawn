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

#include "tint/lang/spirv/writer/ast_printer/scalar_constant.h"
#include "tint/lang/spirv/writer/ast_printer/helper_test.h"

namespace tint::spirv::writer {
namespace {

using SpirvASTPrinterScalarConstantTest = TestHelper;

TEST_F(SpirvASTPrinterScalarConstantTest, Equality) {
    ScalarConstant a{};
    ScalarConstant b{};
    EXPECT_EQ(a, b);

    a.kind = ScalarConstant::Kind::kU32;
    EXPECT_NE(a, b);
    b.kind = ScalarConstant::Kind::kU32;
    EXPECT_EQ(a, b);

    a.value.b = true;
    EXPECT_NE(a, b);
    b.value.b = true;
    EXPECT_EQ(a, b);
}

TEST_F(SpirvASTPrinterScalarConstantTest, U32) {
    auto c = ScalarConstant::U32(123);
    EXPECT_EQ(c.value.u32, 123u);
    EXPECT_EQ(c.kind, ScalarConstant::Kind::kU32);
}

TEST_F(SpirvASTPrinterScalarConstantTest, F16) {
    auto c = ScalarConstant::F16(123.456f);
    // 123.456f will be quantized to f16 123.4375h, bit pattern 0x57b7
    EXPECT_EQ(c.value.f16.bits_representation, 0x57b7u);
    EXPECT_EQ(c.kind, ScalarConstant::Kind::kF16);
}

}  // namespace
}  // namespace tint::spirv::writer
