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
#include "tint/lang/core/constant/scalar.h"
#include "tint/lang/core/ir/block.h"
#include "tint/lang/core/ir/constant.h"
#include "tint/lang/core/ir/var.h"
#include "tint/lang/wgsl/ast/case_selector.h"
#include "tint/lang/wgsl/ast/int_literal_expression.h"
#include "tint/lang/wgsl/helpers/ir_program_test.h"

namespace tint::wgsl {
namespace {

using namespace tint::core::fluent_types;     // NOLINT
using namespace tint::core::number_suffixes;  // NOLINT

using ProgramToIRShadowingTest = helpers::IRProgramTest;

////////////////////////////////////////////////////////////////////////////////
// Shadowing tests
////////////////////////////////////////////////////////////////////////////////
TEST_F(ProgramToIRShadowingTest, Shadow_Struct_With_FnVar) {
    auto m = Build(R"(
struct S {
  i : i32,
}

fn f() -> i32 {
  var S : S = S();
  return S.i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
S = struct @align(4) {
  i:i32 @offset(0)
}

%f = func():i32 -> %b1 {
  %b1 = block {
    %S:ptr<function, S, read_write> = var, S(0i)
    %3:ptr<function, i32, read_write> = access %S, 0u
    %4:i32 = load %3
    ret %4
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_Struct_With_Param) {
    auto m = Build(R"(
struct S {
  i : i32,
}

fn f(S : S) -> i32 {
  return S.i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
S = struct @align(4) {
  i:i32 @offset(0)
}

%f = func(%S:S):i32 -> %b1 {
  %b1 = block {
    %3:i32 = access %S, 0u
    ret %3
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_ModVar_With_FnVar) {
    auto m = Build(R"(
var<private> i : i32 = 1i;

fn f() -> i32 {
  i = (i + 1i);
  var i : i32 = (i + 1i);
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%b1 = block {  # root
  %i:ptr<private, i32, read_write> = var, 1i
}

%f = func():i32 -> %b2 {
  %b2 = block {
    %3:i32 = load %i
    %4:i32 = add %3, 1i
    store %i, %4
    %5:i32 = load %i
    %6:i32 = add %5, 1i
    %i_1:ptr<function, i32, read_write> = var, %6  # %i_1: 'i'
    %8:i32 = load %i_1
    ret %8
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_ModVar_With_FnLet) {
    auto m = Build(R"(
var<private> i : i32 = 1i;

fn f() -> i32 {
  i = (i + 1i);
  let i = (i + 1i);
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%b1 = block {  # root
  %i:ptr<private, i32, read_write> = var, 1i
}

%f = func():i32 -> %b2 {
  %b2 = block {
    %3:i32 = load %i
    %4:i32 = add %3, 1i
    store %i, %4
    %5:i32 = load %i
    %i_1:i32 = add %5, 1i  # %i_1: 'i'
    ret %i_1
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_IfVar) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  if (true) {
    i = (i + 1i);
    var i : i32 = (i + 1i);
    i = (i + 1i);
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    if true [t: %b2] {  # if_1
      %b2 = block {  # true
        %3:i32 = load %i
        %4:i32 = add %3, 1i
        store %i, %4
        %5:i32 = load %i
        %6:i32 = add %5, 1i
        %i_1:ptr<function, i32, read_write> = var, %6  # %i_1: 'i'
        %8:i32 = load %i_1
        %9:i32 = add %8, 1i
        store %i_1, %9
        exit_if  # if_1
      }
    }
    %10:i32 = load %i
    ret %10
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_IfLet) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  if (true) {
    i = (i + 1i);
    let i = (i + 1i);
    return i;
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    if true [t: %b2] {  # if_1
      %b2 = block {  # true
        %3:i32 = load %i
        %4:i32 = add %3, 1i
        store %i, %4
        %5:i32 = load %i
        %i_1:i32 = add %5, 1i  # %i_1: 'i'
        ret %i_1
      }
    }
    %7:i32 = load %i
    ret %7
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_WhileVar) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  while((i < 4i)) {
    var i : i32 = (i + 1i);
    return i;
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [b: %b2, c: %b3] {  # loop_1
      %b2 = block {  # body
        %3:i32 = load %i
        %4:bool = lt %3, 4i
        if %4 [t: %b4, f: %b5] {  # if_1
          %b4 = block {  # true
            exit_if  # if_1
          }
          %b5 = block {  # false
            exit_loop  # loop_1
          }
        }
        %5:i32 = load %i
        %6:i32 = add %5, 1i
        %i_1:ptr<function, i32, read_write> = var, %6  # %i_1: 'i'
        %8:i32 = load %i_1
        ret %8
      }
      %b3 = block {  # continuing
        next_iteration %b2
      }
    }
    %9:i32 = load %i
    ret %9
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_WhileLet) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  while((i < 4i)) {
    let i = (i + 1i);
    return i;
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [b: %b2, c: %b3] {  # loop_1
      %b2 = block {  # body
        %3:i32 = load %i
        %4:bool = lt %3, 4i
        if %4 [t: %b4, f: %b5] {  # if_1
          %b4 = block {  # true
            exit_if  # if_1
          }
          %b5 = block {  # false
            exit_loop  # loop_1
          }
        }
        %5:i32 = load %i
        %i_1:i32 = add %5, 1i  # %i_1: 'i'
        ret %i_1
      }
      %b3 = block {  # continuing
        next_iteration %b2
      }
    }
    %7:i32 = load %i
    ret %7
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_ForInitVar) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  for(var i : f32 = 0.0f; (i < 4.0f); ) {
    let j = i;
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [i: %b2, b: %b3] {  # loop_1
      %b2 = block {  # initializer
        %i_1:ptr<function, f32, read_write> = var, 0.0f  # %i_1: 'i'
        next_iteration %b3
      }
      %b3 = block {  # body
        %4:f32 = load %i_1
        %5:bool = lt %4, 4.0f
        if %5 [t: %b4, f: %b5] {  # if_1
          %b4 = block {  # true
            exit_if  # if_1
          }
          %b5 = block {  # false
            exit_loop  # loop_1
          }
        }
        %j:f32 = load %i_1
        continue %b6
      }
    }
    %7:i32 = load %i
    ret %7
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_ForInitLet) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  for(let i = 0.0f; (i < 4.0f); ) {
    let j = i;
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [i: %b2, b: %b3] {  # loop_1
      %b2 = block {  # initializer
        %i_1:f32 = let 0.0f  # %i_1: 'i'
        next_iteration %b3
      }
      %b3 = block {  # body
        %4:bool = lt %i_1, 4.0f
        if %4 [t: %b4, f: %b5] {  # if_1
          %b4 = block {  # true
            exit_if  # if_1
          }
          %b5 = block {  # false
            exit_loop  # loop_1
          }
        }
        %j:f32 = let %i_1
        continue %b6
      }
    }
    %6:i32 = load %i
    ret %6
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_ForBodyVar) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  for(var x : i32 = 0i; (i < 4i); ) {
    var i : i32 = (i + 1i);
    return i;
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [i: %b2, b: %b3] {  # loop_1
      %b2 = block {  # initializer
        %x:ptr<function, i32, read_write> = var, 0i
        next_iteration %b3
      }
      %b3 = block {  # body
        %4:i32 = load %i
        %5:bool = lt %4, 4i
        if %5 [t: %b4, f: %b5] {  # if_1
          %b4 = block {  # true
            exit_if  # if_1
          }
          %b5 = block {  # false
            exit_loop  # loop_1
          }
        }
        %6:i32 = load %i
        %7:i32 = add %6, 1i
        %i_1:ptr<function, i32, read_write> = var, %7  # %i_1: 'i'
        %9:i32 = load %i_1
        ret %9
      }
    }
    %10:i32 = load %i
    ret %10
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_ForBodyLet) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  for(var x : i32 = 0i; (i < 4i); ) {
    let i = (i + 1i);
    return i;
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [i: %b2, b: %b3] {  # loop_1
      %b2 = block {  # initializer
        %x:ptr<function, i32, read_write> = var, 0i
        next_iteration %b3
      }
      %b3 = block {  # body
        %4:i32 = load %i
        %5:bool = lt %4, 4i
        if %5 [t: %b4, f: %b5] {  # if_1
          %b4 = block {  # true
            exit_if  # if_1
          }
          %b5 = block {  # false
            exit_loop  # loop_1
          }
        }
        %6:i32 = load %i
        %i_1:i32 = add %6, 1i  # %i_1: 'i'
        ret %i_1
      }
    }
    %8:i32 = load %i
    ret %8
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_LoopBodyVar) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  loop {
    if ((i == 2i)) {
      break;
    }
    var i : i32 = (i + 1i);
    if ((i == 3i)) {
      break;
    }
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [b: %b2, c: %b3] {  # loop_1
      %b2 = block {  # body
        %3:i32 = load %i
        %4:bool = eq %3, 2i
        if %4 [t: %b4] {  # if_1
          %b4 = block {  # true
            exit_loop  # loop_1
          }
        }
        %5:i32 = load %i
        %6:i32 = add %5, 1i
        %i_1:ptr<function, i32, read_write> = var, %6  # %i_1: 'i'
        %8:i32 = load %i_1
        %9:bool = eq %8, 3i
        if %9 [t: %b5] {  # if_2
          %b5 = block {  # true
            exit_loop  # loop_1
          }
        }
        continue %b3
      }
      %b3 = block {  # continuing
        next_iteration %b2
      }
    }
    %10:i32 = load %i
    ret %10
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_LoopBodyLet) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  loop {
    if ((i == 2i)) {
      break;
    }
    let i = (i + 1i);
    if ((i == 3i)) {
      break;
    }
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [b: %b2, c: %b3] {  # loop_1
      %b2 = block {  # body
        %3:i32 = load %i
        %4:bool = eq %3, 2i
        if %4 [t: %b4] {  # if_1
          %b4 = block {  # true
            exit_loop  # loop_1
          }
        }
        %5:i32 = load %i
        %i_1:i32 = add %5, 1i  # %i_1: 'i'
        %7:bool = eq %i_1, 3i
        if %7 [t: %b5] {  # if_2
          %b5 = block {  # true
            exit_loop  # loop_1
          }
        }
        continue %b3
      }
      %b3 = block {  # continuing
        next_iteration %b2
      }
    }
    %8:i32 = load %i
    ret %8
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_LoopContinuingVar) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  loop {
    if ((i == 2i)) {
      break;
    }

    continuing {
      var i : i32 = (i + 1i);
      break if (i > 2i);
    }
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [b: %b2, c: %b3] {  # loop_1
      %b2 = block {  # body
        %3:i32 = load %i
        %4:bool = eq %3, 2i
        if %4 [t: %b4] {  # if_1
          %b4 = block {  # true
            exit_loop  # loop_1
          }
        }
        continue %b3
      }
      %b3 = block {  # continuing
        %5:i32 = load %i
        %6:i32 = add %5, 1i
        %i_1:ptr<function, i32, read_write> = var, %6  # %i_1: 'i'
        %8:i32 = load %i_1
        %9:bool = gt %8, 2i
        break_if %9 %b2
      }
    }
    %10:i32 = load %i
    ret %10
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_LoopContinuingLet) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  loop {
    if ((i == 2i)) {
      break;
    }

    continuing {
      let i = (i + 1i);
      break if (i > 2i);
    }
  }
  return i;
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    loop [b: %b2, c: %b3] {  # loop_1
      %b2 = block {  # body
        %3:i32 = load %i
        %4:bool = eq %3, 2i
        if %4 [t: %b4] {  # if_1
          %b4 = block {  # true
            exit_loop  # loop_1
          }
        }
        continue %b3
      }
      %b3 = block {  # continuing
        %5:i32 = load %i
        %i_1:i32 = add %5, 1i  # %i_1: 'i'
        %7:bool = gt %i_1, 2i
        break_if %7 %b2
      }
    }
    %8:i32 = load %i
    ret %8
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_SwitchCaseVar) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  switch(i) {
    case 0i: {
      return i;
    }
    case 1i: {
      var i : i32 = (i + 1i);
      return i;
    }
    default: {
      return i;
    }
  }
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    %3:i32 = load %i
    switch %3 [c: (0i, %b2), c: (1i, %b3), c: (default, %b4)] {  # switch_1
      %b2 = block {  # case
        %4:i32 = load %i
        ret %4
      }
      %b3 = block {  # case
        %5:i32 = load %i
        %6:i32 = add %5, 1i
        %i_1:ptr<function, i32, read_write> = var, %6  # %i_1: 'i'
        %8:i32 = load %i_1
        ret %8
      }
      %b4 = block {  # case
        %9:i32 = load %i
        ret %9
      }
    }
    unreachable
  }
}
)");
}

TEST_F(ProgramToIRShadowingTest, Shadow_FnVar_With_SwitchCaseLet) {
    auto m = Build(R"(
fn f() -> i32 {
  var i : i32;
  switch(i) {
    case 0i: {
      return i;
    }
    case 1i: {
      let i = (i + 1i);
      return i;
    }
    default: {
      return i;
    }
  }
}
)");

    ASSERT_TRUE(m) << m;

    EXPECT_EQ("\n" + Disassemble(m.Get()), R"(
%f = func():i32 -> %b1 {
  %b1 = block {
    %i:ptr<function, i32, read_write> = var
    %3:i32 = load %i
    switch %3 [c: (0i, %b2), c: (1i, %b3), c: (default, %b4)] {  # switch_1
      %b2 = block {  # case
        %4:i32 = load %i
        ret %4
      }
      %b3 = block {  # case
        %5:i32 = load %i
        %i_1:i32 = add %5, 1i  # %i_1: 'i'
        ret %i_1
      }
      %b4 = block {  # case
        %7:i32 = load %i
        ret %7
      }
    }
    unreachable
  }
}
)");
}

}  // namespace
}  // namespace tint::wgsl
