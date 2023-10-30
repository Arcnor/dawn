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

#include "tint/lang/spirv/writer/ast_printer/helper_test.h"
#include "tint/lang/spirv/writer/common/spv_dump_test.h"

using namespace tint::core::number_suffixes;  // NOLINT

namespace tint::spirv::writer {
namespace {

using SpirvASTPrinterTest = TestHelper;

TEST_F(SpirvASTPrinterTest, GlobalConstAssert) {
    GlobalConstAssert(true);

    Builder& b = Build();

    ASSERT_TRUE(b.Build()) << b.Diagnostics();

    // const asserts are not emitted
    EXPECT_EQ(DumpInstructions(b.Module().Types()), "");
    EXPECT_EQ(b.Module().Functions().size(), 0u);
}

TEST_F(SpirvASTPrinterTest, FunctionConstAssert) {
    Func("f", tint::Empty, ty.void_(), Vector{ConstAssert(true)});

    Builder& b = Build();

    ASSERT_TRUE(b.Build()) << b.Diagnostics();

    // const asserts are not emitted
    EXPECT_EQ(DumpInstructions(b.Module().Types()), R"(%2 = OpTypeVoid
%1 = OpTypeFunction %2
)");
    EXPECT_EQ(DumpInstructions(b.Module().Functions()[0].instructions()), R"(OpReturn
)");
}

}  // namespace
}  // namespace tint::spirv::writer
