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

#include "tint/lang/glsl/writer/ast_printer/helper_test.h"
#include "tint/utils/text/string_stream.h"

#include "gmock/gmock.h"

namespace tint::glsl::writer {
namespace {

using namespace tint::core::fluent_types;     // NOLINT
using namespace tint::core::number_suffixes;  // NOLINT

using GlslASTPrinterTest_Cast = TestHelper;

TEST_F(GlslASTPrinterTest_Cast, EmitExpression_Cast_Scalar) {
    auto* cast = Call<f32>(1_i);
    WrapInFunction(cast);

    ASTPrinter& gen = Build();

    StringStream out;
    gen.EmitExpression(out, cast);
    EXPECT_THAT(gen.Diagnostics(), testing::IsEmpty());
    EXPECT_EQ(out.str(), "1.0f");
}

TEST_F(GlslASTPrinterTest_Cast, EmitExpression_Cast_Vector) {
    auto* cast = Call<vec3<f32>>(Call<vec3<i32>>(1_i, 2_i, 3_i));
    WrapInFunction(cast);

    ASTPrinter& gen = Build();

    StringStream out;
    gen.EmitExpression(out, cast);
    EXPECT_THAT(gen.Diagnostics(), testing::IsEmpty());
    EXPECT_EQ(out.str(), "vec3(1.0f, 2.0f, 3.0f)");
}

}  // namespace
}  // namespace tint::glsl::writer
