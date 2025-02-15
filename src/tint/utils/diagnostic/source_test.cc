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

#include "tint/utils/diagnostic/source.h"

#include <memory>
#include <utility>

#include "gtest/gtest.h"

namespace tint {
namespace {

static constexpr const char* kSource = R"(line one
line two
line three)";

using SourceFileContentTest = testing::Test;

TEST_F(SourceFileContentTest, Init) {
    Source::FileContent fc(kSource);
    EXPECT_EQ(fc.data, kSource);
    ASSERT_EQ(fc.lines.size(), 3u);
    EXPECT_EQ(fc.lines[0], "line one");
    EXPECT_EQ(fc.lines[1], "line two");
    EXPECT_EQ(fc.lines[2], "line three");
}

TEST_F(SourceFileContentTest, CopyInit) {
    auto src = std::make_unique<Source::FileContent>(kSource);
    Source::FileContent fc{*src};
    src.reset();
    EXPECT_EQ(fc.data, kSource);
    ASSERT_EQ(fc.lines.size(), 3u);
    EXPECT_EQ(fc.lines[0], "line one");
    EXPECT_EQ(fc.lines[1], "line two");
    EXPECT_EQ(fc.lines[2], "line three");
}

TEST_F(SourceFileContentTest, MoveInit) {
    auto src = std::make_unique<Source::FileContent>(kSource);
    Source::FileContent fc{std::move(*src)};
    src.reset();
    EXPECT_EQ(fc.data, kSource);
    ASSERT_EQ(fc.lines.size(), 3u);
    EXPECT_EQ(fc.lines[0], "line one");
    EXPECT_EQ(fc.lines[1], "line two");
    EXPECT_EQ(fc.lines[2], "line three");
}

// Line break code points
#define kCR "\r"
#define kLF "\n"
#define kVTab "\x0B"
#define kFF "\x0C"
#define kNL "\xC2\x85"
#define kLS "\xE2\x80\xA8"
#define kPS "\xE2\x80\xA9"

using LineBreakTest = testing::TestWithParam<const char*>;
TEST_P(LineBreakTest, Single) {
    std::string src = "line one";
    src += GetParam();
    src += "line two";

    Source::FileContent fc(src);
    EXPECT_EQ(fc.lines.size(), 2u);
    EXPECT_EQ(fc.lines[0], "line one");
    EXPECT_EQ(fc.lines[1], "line two");
}
TEST_P(LineBreakTest, Double) {
    std::string src = "line one";
    src += GetParam();
    src += GetParam();
    src += "line two";

    Source::FileContent fc(src);
    EXPECT_EQ(fc.lines.size(), 3u);
    EXPECT_EQ(fc.lines[0], "line one");
    EXPECT_EQ(fc.lines[1], "");
    EXPECT_EQ(fc.lines[2], "line two");
}
INSTANTIATE_TEST_SUITE_P(SourceFileContentTest,
                         LineBreakTest,
                         testing::Values(kVTab, kFF, kNL, kLS, kPS, kLF, kCR, kCR kLF));

}  // namespace
}  // namespace tint
