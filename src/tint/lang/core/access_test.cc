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

////////////////////////////////////////////////////////////////////////////////
// File generated by 'tools/src/cmd/gen' using the template:
//   src/tint/lang/core/access_test.cc.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "tint/lang/core/access.h"

#include <gtest/gtest.h>

#include <string>

#include "tint/utils/text/string.h"

namespace tint::core {
namespace {

namespace parse_print_tests {

struct Case {
    const char* string;
    Access value;
};

inline std::ostream& operator<<(std::ostream& out, Case c) {
    return out << "'" << std::string(c.string) << "'";
}

static constexpr Case kValidCases[] = {
    {"read", Access::kRead},
    {"read_write", Access::kReadWrite},
    {"write", Access::kWrite},
};

static constexpr Case kInvalidCases[] = {
    {"ccad", Access::kUndefined},       {"3", Access::kUndefined},
    {"rVad", Access::kUndefined},       {"read1write", Access::kUndefined},
    {"reaJqqrite", Access::kUndefined}, {"rea7ll_write", Access::kUndefined},
    {"wrqHtpp", Access::kUndefined},    {"ve", Access::kUndefined},
    {"Grbe", Access::kUndefined},
};

using AccessParseTest = testing::TestWithParam<Case>;

TEST_P(AccessParseTest, Parse) {
    const char* string = GetParam().string;
    Access expect = GetParam().value;
    EXPECT_EQ(expect, ParseAccess(string));
}

INSTANTIATE_TEST_SUITE_P(ValidCases, AccessParseTest, testing::ValuesIn(kValidCases));
INSTANTIATE_TEST_SUITE_P(InvalidCases, AccessParseTest, testing::ValuesIn(kInvalidCases));

using AccessPrintTest = testing::TestWithParam<Case>;

TEST_P(AccessPrintTest, Print) {
    Access value = GetParam().value;
    const char* expect = GetParam().string;
    EXPECT_EQ(expect, tint::ToString(value));
}

INSTANTIATE_TEST_SUITE_P(ValidCases, AccessPrintTest, testing::ValuesIn(kValidCases));

}  // namespace parse_print_tests

}  // namespace
}  // namespace tint::core
