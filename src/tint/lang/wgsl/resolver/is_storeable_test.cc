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

#include "tint/lang/wgsl/resolver/resolver.h"

#include "gmock/gmock.h"
#include "tint/lang/core/type/atomic.h"
#include "tint/lang/wgsl/resolver/resolver_helper_test.h"
#include "tint/lang/wgsl/sem/array.h"

using namespace tint::core::fluent_types;  // NOLINT

namespace tint::resolver {
namespace {

using ResolverIsStorableTest = ResolverTest;

TEST_F(ResolverIsStorableTest, Void) {
    EXPECT_FALSE(r()->IsStorable(create<core::type::Void>()));
}

TEST_F(ResolverIsStorableTest, Scalar) {
    EXPECT_TRUE(r()->IsStorable(create<core::type::Bool>()));
    EXPECT_TRUE(r()->IsStorable(create<core::type::I32>()));
    EXPECT_TRUE(r()->IsStorable(create<core::type::U32>()));
    EXPECT_TRUE(r()->IsStorable(create<core::type::F32>()));
    EXPECT_TRUE(r()->IsStorable(create<core::type::F16>()));
}

TEST_F(ResolverIsStorableTest, Vector) {
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::I32>(), 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::I32>(), 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::I32>(), 4u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::U32>(), 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::U32>(), 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::U32>(), 4u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::F32>(), 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::F32>(), 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::F32>(), 4u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::F16>(), 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::F16>(), 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Vector>(create<core::type::F16>(), 4u)));
}

TEST_F(ResolverIsStorableTest, Matrix) {
    auto* vec2_f32 = create<core::type::Vector>(create<core::type::F32>(), 2u);
    auto* vec3_f32 = create<core::type::Vector>(create<core::type::F32>(), 3u);
    auto* vec4_f32 = create<core::type::Vector>(create<core::type::F32>(), 4u);
    auto* vec2_f16 = create<core::type::Vector>(create<core::type::F16>(), 2u);
    auto* vec3_f16 = create<core::type::Vector>(create<core::type::F16>(), 3u);
    auto* vec4_f16 = create<core::type::Vector>(create<core::type::F16>(), 4u);
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec2_f32, 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec2_f32, 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec2_f32, 4u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec3_f32, 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec3_f32, 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec3_f32, 4u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec4_f32, 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec4_f32, 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec4_f32, 4u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec2_f16, 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec2_f16, 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec2_f16, 4u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec3_f16, 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec3_f16, 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec3_f16, 4u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec4_f16, 2u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec4_f16, 3u)));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Matrix>(vec4_f16, 4u)));
}

TEST_F(ResolverIsStorableTest, Pointer) {
    auto* ptr = create<core::type::Pointer>(core::AddressSpace::kPrivate, create<core::type::I32>(),
                                            core::Access::kReadWrite);
    EXPECT_FALSE(r()->IsStorable(ptr));
}

TEST_F(ResolverIsStorableTest, Atomic) {
    EXPECT_TRUE(r()->IsStorable(create<core::type::Atomic>(create<core::type::I32>())));
    EXPECT_TRUE(r()->IsStorable(create<core::type::Atomic>(create<core::type::U32>())));
}

TEST_F(ResolverIsStorableTest, ArraySizedOfStorable) {
    auto* arr = create<sem::Array>(create<core::type::I32>(),
                                   create<core::type::ConstantArrayCount>(5u), 4u, 20u, 4u, 4u);
    EXPECT_TRUE(r()->IsStorable(arr));
}

TEST_F(ResolverIsStorableTest, ArrayUnsizedOfStorable) {
    auto* arr = create<sem::Array>(create<core::type::I32>(),
                                   create<core::type::RuntimeArrayCount>(), 4u, 4u, 4u, 4u);
    EXPECT_TRUE(r()->IsStorable(arr));
}

TEST_F(ResolverIsStorableTest, Struct_AllMembersStorable) {
    Structure("S", Vector{
                       Member("a", ty.i32()),
                       Member("b", ty.f32()),
                   });

    ASSERT_TRUE(r()->Resolve()) << r()->error();
}

TEST_F(ResolverIsStorableTest, Struct_SomeMembersNonStorable) {
    Structure("S", Vector{
                       Member("a", ty.i32()),
                       Member("b", ty.ptr<private_, i32>()),
                   });

    EXPECT_FALSE(r()->Resolve());
    EXPECT_EQ(
        r()->error(),
        R"(error: ptr<private, i32, read_write> cannot be used as the type of a structure member)");
}

TEST_F(ResolverIsStorableTest, Struct_NestedStorable) {
    auto* storable = Structure("Storable", Vector{
                                               Member("a", ty.i32()),
                                               Member("b", ty.f32()),
                                           });
    Structure("S", Vector{
                       Member("a", ty.i32()),
                       Member("b", ty.Of(storable)),
                   });

    ASSERT_TRUE(r()->Resolve()) << r()->error();
}

TEST_F(ResolverIsStorableTest, Struct_NestedNonStorable) {
    auto* non_storable = Structure("nonstorable", Vector{
                                                      Member("a", ty.i32()),
                                                      Member("b", ty.ptr<private_, i32>()),
                                                  });
    Structure("S", Vector{
                       Member("a", ty.i32()),
                       Member("b", ty.Of(non_storable)),
                   });

    EXPECT_FALSE(r()->Resolve());
    EXPECT_EQ(
        r()->error(),
        R"(error: ptr<private, i32, read_write> cannot be used as the type of a structure member)");
}

}  // namespace
}  // namespace tint::resolver
