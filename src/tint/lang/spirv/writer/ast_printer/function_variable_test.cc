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

#include "tint/lang/spirv/writer/ast_printer/helper_test.h"
#include "tint/lang/spirv/writer/common/spv_dump_test.h"

namespace tint::spirv::writer {
namespace {

using namespace tint::core::fluent_types;     // NOLINT
using namespace tint::core::number_suffixes;  // NOLINT

using SpirvASTPrinterTest = TestHelper;

TEST_F(SpirvASTPrinterTest, FunctionVar_NoAddressSpace) {
    auto* v = Var("var", ty.f32(), core::AddressSpace::kFunction);
    WrapInFunction(v);

    Builder& b = Build();

    b.PushFunctionForTesting();
    EXPECT_TRUE(b.GenerateFunctionVariable(v)) << b.Diagnostics();
    EXPECT_EQ(DumpInstructions(b.Module().Debug()), R"(OpName %1 "var"
)");
    EXPECT_EQ(DumpInstructions(b.Module().Types()), R"(%3 = OpTypeFloat 32
%2 = OpTypePointer Function %3
%4 = OpConstantNull %3
)");

    const auto& func = b.CurrentFunction();
    EXPECT_EQ(DumpInstructions(func.variables()),
              R"(%1 = OpVariable %2 Function %4
)");
}

TEST_F(SpirvASTPrinterTest, FunctionVar_WithConstantInitializer) {
    auto* init = Call<vec3<f32>>(1_f, 1_f, 3_f);
    auto* v = Var("var", ty.vec3<f32>(), core::AddressSpace::kFunction, init);
    WrapInFunction(v);

    Builder& b = Build();

    b.PushFunctionForTesting();
    EXPECT_TRUE(b.GenerateFunctionVariable(v)) << b.Diagnostics();
    ASSERT_FALSE(b.has_error()) << b.Diagnostics();

    EXPECT_EQ(DumpInstructions(b.Module().Debug()), R"(OpName %6 "var"
)");
    EXPECT_EQ(DumpInstructions(b.Module().Types()), R"(%2 = OpTypeFloat 32
%1 = OpTypeVector %2 3
%3 = OpConstant %2 1
%4 = OpConstant %2 3
%5 = OpConstantComposite %1 %3 %3 %4
%7 = OpTypePointer Function %1
%8 = OpConstantNull %1
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().variables()),
              R"(%6 = OpVariable %7 Function %8
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().instructions()),
              R"(OpStore %6 %5
)");
}

TEST_F(SpirvASTPrinterTest, FunctionVar_WithNonConstantInitializer) {
    auto* a = Let("a", Expr(3_f));
    auto* init = Call<vec2<f32>>(1_f, Add(Expr("a"), 3_f));

    auto* v = Var("var", ty.vec2<f32>(), init);
    WrapInFunction(a, v);

    Builder& b = Build();

    b.PushFunctionForTesting();
    EXPECT_TRUE(b.GenerateFunctionVariable(a)) << b.Diagnostics();
    EXPECT_TRUE(b.GenerateFunctionVariable(v)) << b.Diagnostics();
    ASSERT_FALSE(b.has_error()) << b.Diagnostics();

    EXPECT_EQ(DumpInstructions(b.Module().Debug()), R"(OpName %7 "var"
)");
    EXPECT_EQ(DumpInstructions(b.Module().Types()), R"(%1 = OpTypeFloat 32
%2 = OpConstant %1 3
%3 = OpTypeVector %1 2
%4 = OpConstant %1 1
%8 = OpTypePointer Function %3
%9 = OpConstantNull %3
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().variables()),
              R"(%7 = OpVariable %8 Function %9
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().instructions()),
              R"(%5 = OpFAdd %1 %2 %2
%6 = OpCompositeConstruct %3 %4 %5
OpStore %7 %6
)");
}

TEST_F(SpirvASTPrinterTest, FunctionVar_WithNonConstantInitializerLoadedFromVar) {
    // var v : f32 = 1.0;
    // var v2 : f32 = v; // Should generate the load and store automatically.

    auto* v = Var("v", ty.f32(), Expr(1_f));

    auto* v2 = Var("v2", ty.f32(), Expr("v"));
    WrapInFunction(v, v2);

    Builder& b = Build();

    b.PushFunctionForTesting();
    EXPECT_TRUE(b.GenerateFunctionVariable(v)) << b.Diagnostics();
    EXPECT_TRUE(b.GenerateFunctionVariable(v2)) << b.Diagnostics();
    ASSERT_FALSE(b.has_error()) << b.Diagnostics();

    EXPECT_EQ(DumpInstructions(b.Module().Debug()), R"(OpName %3 "v"
OpName %7 "v2"
)");
    EXPECT_EQ(DumpInstructions(b.Module().Types()), R"(%1 = OpTypeFloat 32
%2 = OpConstant %1 1
%4 = OpTypePointer Function %1
%5 = OpConstantNull %1
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().variables()),
              R"(%3 = OpVariable %4 Function %5
%7 = OpVariable %4 Function %5
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().instructions()),
              R"(OpStore %3 %2
%6 = OpLoad %1 %3
OpStore %7 %6
)");
}

TEST_F(SpirvASTPrinterTest, FunctionVar_LetWithVarInitializer) {
    // var v : f32 = 1.0;
    // let v2 : f32 = v; // Should generate the load

    auto* v = Var("v", ty.f32(), Expr(1_f));

    auto* v2 = Var("v2", ty.f32(), Expr("v"));
    WrapInFunction(v, v2);

    Builder& b = Build();

    b.PushFunctionForTesting();
    EXPECT_TRUE(b.GenerateFunctionVariable(v)) << b.Diagnostics();
    EXPECT_TRUE(b.GenerateFunctionVariable(v2)) << b.Diagnostics();
    ASSERT_FALSE(b.has_error()) << b.Diagnostics();

    EXPECT_EQ(DumpInstructions(b.Module().Debug()), R"(OpName %3 "v"
OpName %7 "v2"
)");
    EXPECT_EQ(DumpInstructions(b.Module().Types()), R"(%1 = OpTypeFloat 32
%2 = OpConstant %1 1
%4 = OpTypePointer Function %1
%5 = OpConstantNull %1
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().variables()),
              R"(%3 = OpVariable %4 Function %5
%7 = OpVariable %4 Function %5
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().instructions()),
              R"(OpStore %3 %2
%6 = OpLoad %1 %3
OpStore %7 %6
)");
}

TEST_F(SpirvASTPrinterTest, FunctionVar_ConstWithVarInitializer) {
    // const v : f32 = 1.0;
    // let v2 : f32 = v;

    auto* v = Const("v", ty.f32(), Expr(1_f));

    auto* v2 = Var("v2", ty.f32(), Expr("v"));
    WrapInFunction(v, v2);

    Builder& b = Build();

    b.PushFunctionForTesting();
    EXPECT_TRUE(b.GenerateFunctionVariable(v)) << b.Diagnostics();
    EXPECT_TRUE(b.GenerateFunctionVariable(v2)) << b.Diagnostics();
    ASSERT_FALSE(b.has_error()) << b.Diagnostics();

    EXPECT_EQ(DumpInstructions(b.Module().Debug()), R"(OpName %3 "v2"
)");
    EXPECT_EQ(DumpInstructions(b.Module().Types()), R"(%1 = OpTypeFloat 32
%2 = OpConstant %1 1
%4 = OpTypePointer Function %1
%5 = OpConstantNull %1
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().variables()),
              R"(%3 = OpVariable %4 Function %5
)");
    EXPECT_EQ(DumpInstructions(b.CurrentFunction().instructions()),
              R"(OpStore %3 %2
)");
}

TEST_F(SpirvASTPrinterTest, FunctionVar_Let) {
    auto* init = Call<vec3<f32>>(1_f, 1_f, 3_f);

    auto* v = Let("var", ty.vec3<f32>(), init);

    WrapInFunction(v);

    Builder& b = Build();

    EXPECT_TRUE(b.GenerateFunctionVariable(v)) << b.Diagnostics();
    ASSERT_FALSE(b.has_error()) << b.Diagnostics();

    EXPECT_EQ(DumpInstructions(b.Module().Types()), R"(%2 = OpTypeFloat 32
%1 = OpTypeVector %2 3
%3 = OpConstant %2 1
%4 = OpConstant %2 3
%5 = OpConstantComposite %1 %3 %3 %4
)");
}

TEST_F(SpirvASTPrinterTest, FunctionVar_Const) {
    auto* init = Call<vec3<f32>>(1_f, 1_f, 3_f);

    auto* v = Const("var", ty.vec3<f32>(), init);

    WrapInFunction(v);

    Builder& b = Build();

    EXPECT_TRUE(b.GenerateFunctionVariable(v)) << b.Diagnostics();
    ASSERT_FALSE(b.has_error()) << b.Diagnostics();

    EXPECT_EQ(DumpInstructions(b.Module().Types()), "");  // Not a mistake - 'const' is inlined
}

}  // namespace
}  // namespace tint::spirv::writer
