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

////////////////////////////////////////////////////////////////////////////////
// File generated by 'tools/src/cmd/gen' using the template:
//   src/tint/lang/core/attribute_test.cc.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "tint/lang/core/attribute.h"

#include <gtest/gtest.h>

#include <string>

#include "tint/utils/text/string.h"

namespace tint::core {
namespace {

namespace parse_print_tests {

struct Case {
    const char* string;
    Attribute value;
};

inline std::ostream& operator<<(std::ostream& out, Case c) {
    return out << "'" << std::string(c.string) << "'";
}

static constexpr Case kValidCases[] = {
    {"align", Attribute::kAlign},           {"binding", Attribute::kBinding},
    {"builtin", Attribute::kBuiltin},       {"compute", Attribute::kCompute},
    {"diagnostic", Attribute::kDiagnostic}, {"fragment", Attribute::kFragment},
    {"group", Attribute::kGroup},           {"id", Attribute::kId},
    {"index", Attribute::kIndex},           {"interpolate", Attribute::kInterpolate},
    {"invariant", Attribute::kInvariant},   {"location", Attribute::kLocation},
    {"must_use", Attribute::kMustUse},      {"size", Attribute::kSize},
    {"vertex", Attribute::kVertex},         {"workgroup_size", Attribute::kWorkgroupSize},
};

static constexpr Case kInvalidCases[] = {
    {"alccn", Attribute::kUndefined},
    {"3g", Attribute::kUndefined},
    {"aVign", Attribute::kUndefined},
    {"bind1ng", Attribute::kUndefined},
    {"bqnJing", Attribute::kUndefined},
    {"bindin7ll", Attribute::kUndefined},
    {"ppqqiliHH", Attribute::kUndefined},
    {"bucv", Attribute::kUndefined},
    {"biltGn", Attribute::kUndefined},
    {"compiive", Attribute::kUndefined},
    {"8WWmpute", Attribute::kUndefined},
    {"cxxpute", Attribute::kUndefined},
    {"dXagnosigg", Attribute::kUndefined},
    {"dagnXuVc", Attribute::kUndefined},
    {"diagnosti3", Attribute::kUndefined},
    {"fraEment", Attribute::kUndefined},
    {"PPagTTent", Attribute::kUndefined},
    {"xxragddnt", Attribute::kUndefined},
    {"g44oup", Attribute::kUndefined},
    {"grVVSSp", Attribute::kUndefined},
    {"22RRp", Attribute::kUndefined},
    {"d", Attribute::kUndefined},
    {"i", Attribute::kUndefined},
    {"OVd", Attribute::kUndefined},
    {"ndyx", Attribute::kUndefined},
    {"n77rrldGx", Attribute::kUndefined},
    {"inde40", Attribute::kUndefined},
    {"itooolate", Attribute::kUndefined},
    {"intezplate", Attribute::kUndefined},
    {"ppnerii1olat", Attribute::kUndefined},
    {"invarianXX", Attribute::kUndefined},
    {"inv55ria99nII", Attribute::kUndefined},
    {"irrvariaSSaHH", Attribute::kUndefined},
    {"lkkcin", Attribute::kUndefined},
    {"gjctRRo", Attribute::kUndefined},
    {"lcbton", Attribute::kUndefined},
    {"mustjuse", Attribute::kUndefined},
    {"must_se", Attribute::kUndefined},
    {"muquse", Attribute::kUndefined},
    {"szNN", Attribute::kUndefined},
    {"zvv", Attribute::kUndefined},
    {"QQze", Attribute::kUndefined},
    {"eterf", Attribute::kUndefined},
    {"vertjx", Attribute::kUndefined},
    {"v82wNNx", Attribute::kUndefined},
    {"worgroup_size", Attribute::kUndefined},
    {"workgrourr_size", Attribute::kUndefined},
    {"workgroGp_size", Attribute::kUndefined},
};

using AttributeParseTest = testing::TestWithParam<Case>;

TEST_P(AttributeParseTest, Parse) {
    const char* string = GetParam().string;
    Attribute expect = GetParam().value;
    EXPECT_EQ(expect, ParseAttribute(string));
}

INSTANTIATE_TEST_SUITE_P(ValidCases, AttributeParseTest, testing::ValuesIn(kValidCases));
INSTANTIATE_TEST_SUITE_P(InvalidCases, AttributeParseTest, testing::ValuesIn(kInvalidCases));

using AttributePrintTest = testing::TestWithParam<Case>;

TEST_P(AttributePrintTest, Print) {
    Attribute value = GetParam().value;
    const char* expect = GetParam().string;
    EXPECT_EQ(expect, tint::ToString(value));
}

INSTANTIATE_TEST_SUITE_P(ValidCases, AttributePrintTest, testing::ValuesIn(kValidCases));

}  // namespace parse_print_tests

}  // namespace
}  // namespace tint::core
