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

#include <utility>

#include "tint/lang/core/ir/transform/helper_test.h"
#include "tint/lang/core/type/struct.h"
#include "tint/lang/wgsl/builtin_fn.h"
#include "tint/lang/wgsl/ir/builtin_call.h"
#include "tint/lang/wgsl/reader/lower/lower.h"

namespace tint::wgsl::reader::lower {
namespace {

using namespace tint::core::fluent_types;     // NOLINT
using namespace tint::core::number_suffixes;  // NOLINT

using Wgslreader_LowerTest = core::ir::transform::TransformTest;

TEST_F(Wgslreader_LowerTest, BuiltinConversion) {
    auto* f = b.Function("f", ty.void_());
    b.Append(f->Block(), [&] {  //
        auto* result = b.InstructionResult(ty.i32());
        b.Append(b.ir.instructions.Create<wgsl::ir::BuiltinCall>(result, wgsl::BuiltinFn::kMax,
                                                                 Vector{
                                                                     b.Value(i32(1)),
                                                                     b.Value(i32(2)),
                                                                 }));
        b.Return(f);
    });

    auto* src = R"(
%f = func():void -> %b1 {
  %b1 = block {
    %2:i32 = wgsl.max 1i, 2i
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%f = func():void -> %b1 {
  %b1 = block {
    %2:i32 = max 1i, 2i
    ret
  }
}
)";

    Run(Lower);

    EXPECT_EQ(expect, str());
}

TEST_F(Wgslreader_LowerTest, WorkgroupUniformLoad) {
    auto* wgvar = b.Var("wgvar", ty.ptr<workgroup, i32>());
    mod.root_block->Append(wgvar);

    auto* f = b.Function("f", ty.i32());
    b.Append(f->Block(), [&] {  //
        auto* result = b.InstructionResult(ty.i32());
        b.Append(b.ir.instructions.Create<wgsl::ir::BuiltinCall>(
            result, wgsl::BuiltinFn::kWorkgroupUniformLoad, Vector{wgvar->Result()}));
        b.Return(f, result);
    });

    auto* src = R"(
%b1 = block {  # root
  %wgvar:ptr<workgroup, i32, read_write> = var
}

%f = func():i32 -> %b2 {
  %b2 = block {
    %3:i32 = wgsl.workgroupUniformLoad %wgvar
    ret %3
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
%b1 = block {  # root
  %wgvar:ptr<workgroup, i32, read_write> = var
}

%f = func():i32 -> %b2 {
  %b2 = block {
    %3:void = workgroupBarrier
    %4:i32 = load %wgvar
    %5:void = workgroupBarrier
    ret %4
  }
}
)";

    Run(Lower);

    EXPECT_EQ(expect, str());
}

}  // namespace
}  // namespace tint::wgsl::reader::lower
