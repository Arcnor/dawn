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

#include "tint/lang/spirv/writer/ast_raise/clamp_frag_depth.h"

#include "tint/lang/wgsl/ast/transform/helper_test.h"

namespace tint::spirv::writer {
namespace {

using ClampFragDepthTest = ast::transform::TransformTest;

TEST_F(ClampFragDepthTest, ShouldRunEmptyModule) {
    auto* src = R"()";

    EXPECT_FALSE(ShouldRun<ClampFragDepth>(src));
}

TEST_F(ClampFragDepthTest, ShouldRunNoFragmentShader) {
    auto* src = R"(
        fn f() -> f32 {
            return 0.0;
        }

        @compute @workgroup_size(1) fn cs() {
        }

        @vertex fn vs() -> @builtin(position) vec4<f32> {
            return vec4<f32>();
        }
    )";

    EXPECT_FALSE(ShouldRun<ClampFragDepth>(src));
}

TEST_F(ClampFragDepthTest, ShouldRunFragmentShaderNoReturnType) {
    auto* src = R"(
        @fragment fn main() {
        }
    )";

    EXPECT_FALSE(ShouldRun<ClampFragDepth>(src));
}

TEST_F(ClampFragDepthTest, ShouldRunFragmentShaderNoFragDepth) {
    auto* src = R"(
        @fragment fn main() -> @location(0) f32 {
            return 0.0;
        }

        struct S {
            @location(0) a : f32,
            @builtin(sample_mask) b : u32,
        }
        @fragment fn main2() -> S {
            return S();
        }
    )";

    EXPECT_FALSE(ShouldRun<ClampFragDepth>(src));
}

TEST_F(ClampFragDepthTest, ShouldRunFragDepthAsDirectReturn) {
    auto* src = R"(
        @fragment fn main() -> @builtin(frag_depth) f32 {
            return 0.0;
        }
    )";

    EXPECT_TRUE(ShouldRun<ClampFragDepth>(src));
}

TEST_F(ClampFragDepthTest, ShouldRunFragDepthInStruct) {
    auto* src = R"(
        struct S {
            @location(0) a : f32,
            @builtin(frag_depth) b : f32,
            @location(1) c : f32,
        }
        @fragment fn main() -> S {
            return S();
        }
    )";

    EXPECT_TRUE(ShouldRun<ClampFragDepth>(src));
}

TEST_F(ClampFragDepthTest, SingleReturnOfFragDepth) {
    auto* src = R"(
        @fragment fn main() -> @builtin(frag_depth) f32 {
            return 0.0;
        }
    )";

    auto* expect = R"(
enable chromium_experimental_push_constant;

struct FragDepthClampArgs {
  min : f32,
  max : f32,
}

var<push_constant> frag_depth_clamp_args : FragDepthClampArgs;

fn clamp_frag_depth(v : f32) -> f32 {
  return clamp(v, frag_depth_clamp_args.min, frag_depth_clamp_args.max);
}

@fragment
fn main() -> @builtin(frag_depth) f32 {
  return clamp_frag_depth(0.0);
}
)";

    auto got = Run<ClampFragDepth>(src);
    EXPECT_EQ(expect, str(got));
}

TEST_F(ClampFragDepthTest, MultipleReturnOfFragDepth) {
    auto* src = R"(
        @fragment fn main() -> @builtin(frag_depth) f32 {
            if (false) {
                return 1.0;
            }
            return 0.0;
        }
    )";

    auto* expect = R"(
enable chromium_experimental_push_constant;

struct FragDepthClampArgs {
  min : f32,
  max : f32,
}

var<push_constant> frag_depth_clamp_args : FragDepthClampArgs;

fn clamp_frag_depth(v : f32) -> f32 {
  return clamp(v, frag_depth_clamp_args.min, frag_depth_clamp_args.max);
}

@fragment
fn main() -> @builtin(frag_depth) f32 {
  if (false) {
    return clamp_frag_depth(1.0);
  }
  return clamp_frag_depth(0.0);
}
)";

    auto got = Run<ClampFragDepth>(src);
    EXPECT_EQ(expect, str(got));
}

TEST_F(ClampFragDepthTest, OtherFunctionWithoutFragDepth) {
    auto* src = R"(
        @fragment fn main() -> @builtin(frag_depth) f32 {
            return 0.0;
        }
        @fragment fn other() -> @location(0) f32 {
            return 0.0;
        }
    )";

    auto* expect = R"(
enable chromium_experimental_push_constant;

struct FragDepthClampArgs {
  min : f32,
  max : f32,
}

var<push_constant> frag_depth_clamp_args : FragDepthClampArgs;

fn clamp_frag_depth(v : f32) -> f32 {
  return clamp(v, frag_depth_clamp_args.min, frag_depth_clamp_args.max);
}

@fragment
fn main() -> @builtin(frag_depth) f32 {
  return clamp_frag_depth(0.0);
}

@fragment
fn other() -> @location(0) f32 {
  return 0.0;
}
)";

    auto got = Run<ClampFragDepth>(src);
    EXPECT_EQ(expect, str(got));
}

TEST_F(ClampFragDepthTest, SimpleReturnOfStruct) {
    auto* src = R"(
        struct S {
            @builtin(frag_depth) frag_depth : f32,
        }

        @fragment fn main() -> S {
            return S(0.0);
        }
    )";

    auto* expect = R"(
enable chromium_experimental_push_constant;

struct FragDepthClampArgs {
  min : f32,
  max : f32,
}

var<push_constant> frag_depth_clamp_args : FragDepthClampArgs;

fn clamp_frag_depth(v : f32) -> f32 {
  return clamp(v, frag_depth_clamp_args.min, frag_depth_clamp_args.max);
}

struct S {
  @builtin(frag_depth)
  frag_depth : f32,
}

fn clamp_frag_depth_S(s : S) -> S {
  return S(clamp_frag_depth(s.frag_depth));
}

@fragment
fn main() -> S {
  return clamp_frag_depth_S(S(0.0));
}
)";

    auto got = Run<ClampFragDepth>(src);
    EXPECT_EQ(expect, str(got));
}

TEST_F(ClampFragDepthTest, MixOfFunctionReturningStruct) {
    auto* src = R"(
        struct S {
            @builtin(frag_depth) frag_depth : f32,
        }
        struct S2 {
            @builtin(frag_depth) frag_depth : f32,
        }

        @fragment fn returnS() -> S {
            return S(0.0);
        }
        @fragment fn againReturnS() -> S {
            return S(0.0);
        }
        @fragment fn returnS2() -> S2 {
            return S2(0.0);
        }
    )";

    // clamp_frag_depth_S is emitted only once.
    // S2 gets its own clamping function.
    auto* expect = R"(
enable chromium_experimental_push_constant;

struct FragDepthClampArgs {
  min : f32,
  max : f32,
}

var<push_constant> frag_depth_clamp_args : FragDepthClampArgs;

fn clamp_frag_depth(v : f32) -> f32 {
  return clamp(v, frag_depth_clamp_args.min, frag_depth_clamp_args.max);
}

struct S {
  @builtin(frag_depth)
  frag_depth : f32,
}

struct S2 {
  @builtin(frag_depth)
  frag_depth : f32,
}

fn clamp_frag_depth_S(s : S) -> S {
  return S(clamp_frag_depth(s.frag_depth));
}

@fragment
fn returnS() -> S {
  return clamp_frag_depth_S(S(0.0));
}

@fragment
fn againReturnS() -> S {
  return clamp_frag_depth_S(S(0.0));
}

fn clamp_frag_depth_S2(s : S2) -> S2 {
  return S2(clamp_frag_depth(s.frag_depth));
}

@fragment
fn returnS2() -> S2 {
  return clamp_frag_depth_S2(S2(0.0));
}
)";

    auto got = Run<ClampFragDepth>(src);
    EXPECT_EQ(expect, str(got));
}

TEST_F(ClampFragDepthTest, ComplexIOStruct) {
    auto* src = R"(
        struct S {
            @location(0) blou : vec4<f32>,
            @location(1) bi : vec4<f32>,
            @builtin(frag_depth) frag_depth : f32,
            @location(2) boul : i32,
            @builtin(sample_mask) ga : u32,
        }

        @fragment fn main() -> S {
            return S(vec4<f32>(), vec4<f32>(), 0.0, 1, 0u);
        }
    )";

    auto* expect = R"(
enable chromium_experimental_push_constant;

struct FragDepthClampArgs {
  min : f32,
  max : f32,
}

var<push_constant> frag_depth_clamp_args : FragDepthClampArgs;

fn clamp_frag_depth(v : f32) -> f32 {
  return clamp(v, frag_depth_clamp_args.min, frag_depth_clamp_args.max);
}

struct S {
  @location(0)
  blou : vec4<f32>,
  @location(1)
  bi : vec4<f32>,
  @builtin(frag_depth)
  frag_depth : f32,
  @location(2)
  boul : i32,
  @builtin(sample_mask)
  ga : u32,
}

fn clamp_frag_depth_S(s : S) -> S {
  return S(s.blou, s.bi, clamp_frag_depth(s.frag_depth), s.boul, s.ga);
}

@fragment
fn main() -> S {
  return clamp_frag_depth_S(S(vec4<f32>(), vec4<f32>(), 0.0, 1, 0u));
}
)";

    auto got = Run<ClampFragDepth>(src);
    EXPECT_EQ(expect, str(got));
}

}  // namespace
}  // namespace tint::spirv::writer
