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

#include "tint/lang/core/type/depth_multisampled_texture.h"

#include "tint/lang/core/type/manager.h"
#include "tint/lang/core/type/texture_dimension.h"
#include "tint/utils/diagnostic/diagnostic.h"
#include "tint/utils/ice/ice.h"
#include "tint/utils/math/hash.h"
#include "tint/utils/text/string_stream.h"

TINT_INSTANTIATE_TYPEINFO(tint::core::type::DepthMultisampledTexture);

namespace tint::core::type {
namespace {

bool IsValidDepthDimension(TextureDimension dim) {
    return dim == TextureDimension::k2d;
}

}  // namespace

DepthMultisampledTexture::DepthMultisampledTexture(TextureDimension dim)
    : Base(Hash(TypeInfo::Of<DepthMultisampledTexture>().full_hashcode, dim), dim) {
    TINT_ASSERT(IsValidDepthDimension(dim));
}

DepthMultisampledTexture::~DepthMultisampledTexture() = default;

bool DepthMultisampledTexture::Equals(const UniqueNode& other) const {
    if (auto* o = other.As<DepthMultisampledTexture>()) {
        return o->dim() == dim();
    }
    return false;
}

std::string DepthMultisampledTexture::FriendlyName() const {
    StringStream out;
    out << "texture_depth_multisampled_" << dim();
    return out.str();
}

DepthMultisampledTexture* DepthMultisampledTexture::Clone(CloneContext& ctx) const {
    return ctx.dst.mgr->Get<DepthMultisampledTexture>(dim());
}

}  // namespace tint::core::type
