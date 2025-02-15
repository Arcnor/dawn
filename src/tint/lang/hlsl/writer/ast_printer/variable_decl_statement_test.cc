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
#include "gmock/gmock.h"
#include "tint/lang/hlsl/writer/ast_printer/helper_test.h"

namespace tint::hlsl::writer {
namespace {

using namespace tint::core::fluent_types;     // NOLINT
using namespace tint::core::number_suffixes;  // NOLINT

using ::testing::HasSubstr;

using HlslASTPrinterTest_VariableDecl = TestHelper;

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement) {
    auto* var = Var("a", ty.f32());
    auto* stmt = Decl(var);
    WrapInFunction(stmt);

    ASTPrinter& gen = Build();

    gen.IncrementIndent();

    ASSERT_TRUE(gen.EmitStatement(stmt)) << gen.Diagnostics();
    EXPECT_EQ(gen.Result(), "  float a = 0.0f;\n");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Let) {
    auto* var = Let("a", ty.f32(), Call<f32>());
    auto* stmt = Decl(var);
    WrapInFunction(stmt);

    ASTPrinter& gen = Build();

    gen.IncrementIndent();

    ASSERT_TRUE(gen.EmitStatement(stmt)) << gen.Diagnostics();
    EXPECT_EQ(gen.Result(), "  const float a = 0.0f;\n");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const) {
    auto* var = Const("a", ty.f32(), Call<f32>());
    auto* stmt = Decl(var);
    WrapInFunction(stmt);

    ASTPrinter& gen = Build();

    gen.IncrementIndent();

    ASSERT_TRUE(gen.EmitStatement(stmt)) << gen.Diagnostics();
    EXPECT_EQ(gen.Result(), "");  // Not a mistake - 'const' is inlined
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_AInt) {
    auto* C = Const("C", Expr(1_a));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const int l = 1;
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_AFloat) {
    auto* C = Const("C", Expr(1._a));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const float l = 1.0f;
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_i32) {
    auto* C = Const("C", Expr(1_i));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const int l = 1;
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_u32) {
    auto* C = Const("C", Expr(1_u));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const uint l = 1u;
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_f32) {
    auto* C = Const("C", Expr(1_f));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const float l = 1.0f;
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_f16) {
    Enable(wgsl::Extension::kF16);

    auto* C = Const("C", Expr(1_h));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const float16_t l = float16_t(1.0h);
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_vec3_AInt) {
    auto* C = Const("C", Call<vec3<Infer>>(1_a, 2_a, 3_a));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const int3 l = int3(1, 2, 3);
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_vec3_AFloat) {
    auto* C = Const("C", Call<vec3<Infer>>(1._a, 2._a, 3._a));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const float3 l = float3(1.0f, 2.0f, 3.0f);
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_vec3_f32) {
    auto* C = Const("C", Call<vec3<f32>>(1_f, 2_f, 3_f));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const float3 l = float3(1.0f, 2.0f, 3.0f);
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_vec3_f16) {
    Enable(wgsl::Extension::kF16);

    auto* C = Const("C", Call<vec3<f16>>(1_h, 2_h, 3_h));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const vector<float16_t, 3> l = vector<float16_t, 3>(float16_t(1.0h), float16_t(2.0h), float16_t(3.0h));
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_mat2x3_AFloat) {
    auto* C = Const("C", Call<mat2x3<Infer>>(1._a, 2._a, 3._a, 4._a, 5._a, 6._a));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const float2x3 l = float2x3(float3(1.0f, 2.0f, 3.0f), float3(4.0f, 5.0f, 6.0f));
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_mat2x3_f32) {
    auto* C = Const("C", Call<mat2x3<f32>>(1_f, 2_f, 3_f, 4_f, 5_f, 6_f));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const float2x3 l = float2x3(float3(1.0f, 2.0f, 3.0f), float3(4.0f, 5.0f, 6.0f));
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_mat2x3_f16) {
    Enable(wgsl::Extension::kF16);

    auto* C = Const("C", Call<mat2x3<f16>>(1_h, 2_h, 3_h, 4_h, 5_h, 6_h));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const matrix<float16_t, 2, 3> l = matrix<float16_t, 2, 3>(vector<float16_t, 3>(float16_t(1.0h), float16_t(2.0h), float16_t(3.0h)), vector<float16_t, 3>(float16_t(4.0h), float16_t(5.0h), float16_t(6.0h)));
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_arr_f32) {
    auto* C = Const("C", Call(ty.array<f32, 3>(), 1_f, 2_f, 3_f));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const float l[3] = {1.0f, 2.0f, 3.0f};
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_arr_vec2_bool) {
    auto* C = Const("C", Call<array<vec2<bool>, 3>>(         //
                             Call<vec2<bool>>(true, false),  //
                             Call<vec2<bool>>(false, true),  //
                             Call<vec2<bool>>(true, true)));
    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const bool2 l[3] = {bool2(true, false), bool2(false, true), (true).xx};
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Array) {
    auto* var = Var("a", ty.array<f32, 5>());

    WrapInFunction(var, Expr("a"));

    ASTPrinter& gen = Build();

    gen.IncrementIndent();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();
    EXPECT_THAT(gen.Result(), HasSubstr("  float a[5] = (float[5])0;\n"));
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Private) {
    GlobalVar("a", ty.f32(), core::AddressSpace::kPrivate);

    WrapInFunction(Expr("a"));

    ASTPrinter& gen = Build();

    gen.IncrementIndent();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();
    EXPECT_THAT(gen.Result(), HasSubstr("  static float a = 0.0f;\n"));
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Initializer_ZeroVec_F32) {
    auto* var = Var("a", ty.vec3<f32>(), Call<vec3<f32>>());

    auto* stmt = Decl(var);
    WrapInFunction(stmt);

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.EmitStatement(stmt)) << gen.Diagnostics();
    EXPECT_EQ(gen.Result(), R"(float3 a = (0.0f).xxx;
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Initializer_ZeroVec_F16) {
    Enable(wgsl::Extension::kF16);

    auto* var = Var("a", ty.vec3<f16>(), Call<vec3<f16>>());

    auto* stmt = Decl(var);
    WrapInFunction(stmt);

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.EmitStatement(stmt)) << gen.Diagnostics();
    EXPECT_EQ(gen.Result(), R"(vector<float16_t, 3> a = (float16_t(0.0h)).xxx;
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Initializer_ZeroMat_F32) {
    auto* var = Var("a", ty.mat2x3<f32>(), Call<mat2x3<f32>>());

    auto* stmt = Decl(var);
    WrapInFunction(stmt);

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.EmitStatement(stmt)) << gen.Diagnostics();
    EXPECT_EQ(gen.Result(),
              R"(float2x3 a = float2x3((0.0f).xxx, (0.0f).xxx);
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Initializer_ZeroMat_F16) {
    Enable(wgsl::Extension::kF16);

    auto* var = Var("a", ty.mat2x3<f16>(), Call<mat2x3<f16>>());

    auto* stmt = Decl(var);
    WrapInFunction(stmt);

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.EmitStatement(stmt)) << gen.Diagnostics();
    EXPECT_EQ(
        gen.Result(),
        R"(matrix<float16_t, 2, 3> a = matrix<float16_t, 2, 3>((float16_t(0.0h)).xxx, (float16_t(0.0h)).xxx);
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_Mat) {
    auto* C = Const("C", Call<mat2x3<f32>>(1_f, 2_f, 3_f, 4_f, 5_f, 6_f));

    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  const float2x3 l = float2x3(float3(1.0f, 2.0f, 3.0f), float3(4.0f, 5.0f, 6.0f));
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_Struct_of_Mat) {
    Structure("S", Vector{Member("m", ty.mat2x3<f32>())});
    auto* C = Const("C", Call("S", Call<mat2x3<f32>>(1_f, 2_f, 3_f, 4_f, 5_f, 6_f)));

    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(struct S {
  float2x3 m;
};

void f() {
  S l = {float2x3(float3(1.0f, 2.0f, 3.0f), float3(4.0f, 5.0f, 6.0f))};
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_Struct_of_Struct_of_Mat) {
    Structure("S", Vector{Member("m", ty.mat2x3<f32>())});
    Structure("S2", Vector{Member("s", ty("S"))});
    auto* C = Const("C", Call("S2", Call("S", Call<mat2x3<f32>>(1_f, 2_f, 3_f, 4_f, 5_f, 6_f))));

    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(struct S {
  float2x3 m;
};
struct S2 {
  S s;
};

void f() {
  S2 l = {{float2x3(float3(1.0f, 2.0f, 3.0f), float3(4.0f, 5.0f, 6.0f))}};
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_Struct_of_Array_of_Mat) {
    Structure("S", Vector{Member("m", ty.array(ty.mat2x3<f32>(), 1_u))});

    auto* C = Const("C", Call("S", Call(ty.array(ty.mat2x3<f32>(), 1_u),
                                        Call<mat2x3<f32>>(1_f, 2_f, 3_f, 4_f, 5_f, 6_f))));

    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(struct S {
  float2x3 m[1];
};

void f() {
  S l = {{float2x3(float3(1.0f, 2.0f, 3.0f), float3(4.0f, 5.0f, 6.0f))}};
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_Array_of_Mat) {
    auto* C = Const("C", Call(ty.array(ty.mat2x3<f32>(), 1_u),
                              Call<mat2x3<f32>>(1_f, 2_f, 3_f, 4_f, 5_f, 6_f)));

    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(void f() {
  float2x3 l[1] = {float2x3(float3(1.0f, 2.0f, 3.0f), float3(4.0f, 5.0f, 6.0f))};
}
)");
}

TEST_F(HlslASTPrinterTest_VariableDecl, Emit_VariableDeclStatement_Const_Array_of_Struct_of_Mat) {
    Structure("S", Vector{Member("m", ty.mat2x3<f32>())});

    auto* C = Const("C", Call(ty.array(ty("S"), 1_u),
                              Call(ty("S"), Call<mat2x3<f32>>(1_f, 2_f, 3_f, 4_f, 5_f, 6_f))));

    Func("f", tint::Empty, ty.void_(),
         Vector{
             Decl(C),
             Decl(Let("l", Expr(C))),
         });

    ASTPrinter& gen = Build();

    ASSERT_TRUE(gen.Generate()) << gen.Diagnostics();

    EXPECT_EQ(gen.Result(), R"(struct S {
  float2x3 m;
};

void f() {
  S l[1] = {{float2x3(float3(1.0f, 2.0f, 3.0f), float3(4.0f, 5.0f, 6.0f))}};
}
)");
}

}  // namespace
}  // namespace tint::hlsl::writer
