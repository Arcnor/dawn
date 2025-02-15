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

#include "gmock/gmock.h"
#include "tint/lang/spirv/reader/ast_parser/helper_test.h"
#include "tint/lang/spirv/reader/ast_parser/spirv_tools_helpers_test.h"

namespace tint::spirv::reader::ast_parser {
namespace {

using ::testing::HasSubstr;

TEST_F(SpirvASTParserTest, Impl_Uint32VecEmpty) {
    std::vector<uint32_t> data;
    auto p = parser(data);
    EXPECT_FALSE(p->Parse());
    // TODO(dneto): What message?
}

TEST_F(SpirvASTParserTest, Impl_InvalidModuleFails) {
    auto invalid_spv = test::Assemble("%ty = OpTypeInt 3 0");
    auto p = parser(invalid_spv);
    EXPECT_FALSE(p->Parse());
    EXPECT_THAT(p->error(), HasSubstr("TypeInt cannot appear before the memory model instruction"));
    EXPECT_THAT(p->error(), HasSubstr("OpTypeInt 3 0"));
}

TEST_F(SpirvASTParserTest, Impl_GenericVulkanShader_SimpleMemoryModel) {
    auto spv = test::Assemble(R"(
  OpCapability Shader
  OpMemoryModel Logical Simple
  OpEntryPoint GLCompute %main "main"
  OpExecutionMode %main LocalSize 1 1 1
  %void = OpTypeVoid
  %voidfn = OpTypeFunction %void
  %main = OpFunction %void None %voidfn
  %entry = OpLabel
  OpReturn
  OpFunctionEnd
)");
    auto p = parser(spv);
    EXPECT_TRUE(p->Parse());
    EXPECT_TRUE(p->error().empty());
}

TEST_F(SpirvASTParserTest, Impl_GenericVulkanShader_GLSL450MemoryModel) {
    auto spv = test::Assemble(R"(
  OpCapability Shader
  OpMemoryModel Logical GLSL450
  OpEntryPoint GLCompute %main "main"
  OpExecutionMode %main LocalSize 1 1 1
  %void = OpTypeVoid
  %voidfn = OpTypeFunction %void
  %main = OpFunction %void None %voidfn
  %entry = OpLabel
  OpReturn
  OpFunctionEnd
)");
    auto p = parser(spv);
    EXPECT_TRUE(p->Parse());
    EXPECT_TRUE(p->error().empty());
}

TEST_F(SpirvASTParserTest, Impl_GenericVulkanShader_VulkanMemoryModel) {
    auto spv = test::Assemble(R"(
  OpCapability Shader
  OpCapability VulkanMemoryModelKHR
  OpExtension "SPV_KHR_vulkan_memory_model"
  OpMemoryModel Logical VulkanKHR
  OpEntryPoint GLCompute %main "main"
  OpExecutionMode %main LocalSize 1 1 1
  %void = OpTypeVoid
  %voidfn = OpTypeFunction %void
  %main = OpFunction %void None %voidfn
  %entry = OpLabel
  OpReturn
  OpFunctionEnd
)");
    auto p = parser(spv);
    EXPECT_TRUE(p->Parse());
    EXPECT_TRUE(p->error().empty());
}

TEST_F(SpirvASTParserTest, Impl_OpenCLKernel_Fails) {
    auto spv = test::Assemble(R"(
  OpCapability Kernel
  OpCapability Addresses
  OpMemoryModel Physical32 OpenCL
  OpEntryPoint Kernel %main "main"
  %void = OpTypeVoid
  %voidfn = OpTypeFunction %void
  %main = OpFunction %void None %voidfn
  %entry = OpLabel
  OpReturn
  OpFunctionEnd
)");
    auto p = parser(spv);
    EXPECT_FALSE(p->Parse());
    EXPECT_THAT(p->error(), HasSubstr("Capability Kernel is not allowed"));
}

TEST_F(SpirvASTParserTest, Impl_Source_NoOpLine) {
    auto spv = test::Assemble(R"(
  OpCapability Shader
  OpMemoryModel Logical Simple
  OpEntryPoint GLCompute %main "main"
  OpExecutionMode %main LocalSize 1 1 1
  %void = OpTypeVoid
  %voidfn = OpTypeFunction %void
  %5 = OpTypeInt 32 0
  %60 = OpConstantNull %5
  %main = OpFunction %void None %voidfn
  %1 = OpLabel
  OpReturn
  OpFunctionEnd
)");
    auto p = parser(spv);
    EXPECT_TRUE(p->Parse());
    EXPECT_TRUE(p->error().empty());
    // Use instruction counting.
    auto s5 = p->GetSourceForResultIdForTest(5);
    EXPECT_EQ(7u, s5.range.begin.line);
    EXPECT_EQ(0u, s5.range.begin.column);
    auto s60 = p->GetSourceForResultIdForTest(60);
    EXPECT_EQ(8u, s60.range.begin.line);
    EXPECT_EQ(0u, s60.range.begin.column);
    auto s1 = p->GetSourceForResultIdForTest(1);
    EXPECT_EQ(10u, s1.range.begin.line);
    EXPECT_EQ(0u, s1.range.begin.column);
}

TEST_F(SpirvASTParserTest, Impl_Source_WithOpLine_WithOpNoLine) {
    auto spv = test::Assemble(R"(
  OpCapability Shader
  OpMemoryModel Logical Simple
  OpEntryPoint GLCompute %main "main"
  OpExecutionMode %main LocalSize 1 1 1
  %15 = OpString "myfile"
  %void = OpTypeVoid
  %voidfn = OpTypeFunction %void
  OpLine %15 42 53
  %5 = OpTypeInt 32 0
  %60 = OpConstantNull %5
  OpNoLine
  %main = OpFunction %void None %voidfn
  %1 = OpLabel
  OpReturn
  OpFunctionEnd
)");
    auto p = parser(spv);
    EXPECT_TRUE(p->Parse());
    EXPECT_TRUE(p->error().empty());
    // Use the information from the OpLine that is still in scope.
    auto s5 = p->GetSourceForResultIdForTest(5);
    EXPECT_EQ(42u, s5.range.begin.line);
    EXPECT_EQ(53u, s5.range.begin.column);
    auto s60 = p->GetSourceForResultIdForTest(60);
    EXPECT_EQ(42u, s60.range.begin.line);
    EXPECT_EQ(53u, s60.range.begin.column);
    // After OpNoLine, revert back to instruction counting.
    auto s1 = p->GetSourceForResultIdForTest(1);
    EXPECT_EQ(14u, s1.range.begin.line);
    EXPECT_EQ(0u, s1.range.begin.column);
}

TEST_F(SpirvASTParserTest, Impl_Source_InvalidId) {
    auto spv = test::Assemble(R"(
  OpCapability Shader
  OpMemoryModel Logical Simple
  OpEntryPoint GLCompute %main "main"
  OpExecutionMode %main LocalSize 1 1 1
  %15 = OpString "myfile"
  %void = OpTypeVoid
  %voidfn = OpTypeFunction %void
  %main = OpFunction %void None %voidfn
  %1 = OpLabel
  OpReturn
  OpFunctionEnd
)");
    auto p = parser(spv);
    EXPECT_TRUE(p->Parse());
    EXPECT_TRUE(p->error().empty());
    auto s99 = p->GetSourceForResultIdForTest(99);
    EXPECT_EQ(0u, s99.range.begin.line);
    EXPECT_EQ(0u, s99.range.begin.column);
}

TEST_F(SpirvASTParserTest, Impl_IsValidIdentifier) {
    EXPECT_FALSE(ASTParser::IsValidIdentifier(""));  // empty
    EXPECT_FALSE(ASTParser::IsValidIdentifier("_"));
    EXPECT_FALSE(ASTParser::IsValidIdentifier("__"));
    EXPECT_TRUE(ASTParser::IsValidIdentifier("_x"));
    EXPECT_FALSE(ASTParser::IsValidIdentifier("9"));    // leading digit, but ok later
    EXPECT_FALSE(ASTParser::IsValidIdentifier(" "));    // leading space
    EXPECT_FALSE(ASTParser::IsValidIdentifier("a "));   // trailing space
    EXPECT_FALSE(ASTParser::IsValidIdentifier("a 1"));  // space in the middle
    EXPECT_FALSE(ASTParser::IsValidIdentifier("."));    // weird character

    // a simple identifier
    EXPECT_TRUE(ASTParser::IsValidIdentifier("A"));
    // each upper case letter
    EXPECT_TRUE(ASTParser::IsValidIdentifier("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    // each lower case letter
    EXPECT_TRUE(ASTParser::IsValidIdentifier("abcdefghijklmnopqrstuvwxyz"));
    EXPECT_TRUE(ASTParser::IsValidIdentifier("a0123456789"));  // each digit
    EXPECT_TRUE(ASTParser::IsValidIdentifier("x_"));           // has underscore
}

TEST_F(SpirvASTParserTest, Impl_FailOnNonFiniteLiteral) {
    auto spv = test::Assemble(R"(
                       OpCapability Shader
                       OpMemoryModel Logical GLSL450
                       OpEntryPoint Fragment %main "main" %out_var_SV_TARGET
                       OpExecutionMode %main OriginUpperLeft
                       OpSource HLSL 600
                       OpName %out_var_SV_TARGET "out.var.SV_TARGET"
                       OpName %main "main"
                       OpDecorate %out_var_SV_TARGET Location 0
              %float = OpTypeFloat 32
     %float_0x1p_128 = OpConstant %float -0x1p+128
            %v4float = OpTypeVector %float 4
%_ptr_Output_v4float = OpTypePointer Output %v4float
               %void = OpTypeVoid
                  %9 = OpTypeFunction %void
  %out_var_SV_TARGET = OpVariable %_ptr_Output_v4float Output
               %main = OpFunction %void None %9
                 %10 = OpLabel
                 %12 = OpCompositeConstruct %v4float %float_0x1p_128 %float_0x1p_128 %float_0x1p_128 %float_0x1p_128
                       OpStore %out_var_SV_TARGET %12
                       OpReturn
                       OpFunctionEnd

)");
    auto p = parser(spv);
    EXPECT_FALSE(p->Parse());
    EXPECT_THAT(p->error(), HasSubstr("value cannot be represented as 'f32': -inf"));
}

}  // namespace
}  // namespace tint::spirv::reader::ast_parser
