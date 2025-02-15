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

#include "tint/lang/wgsl/resolver/incomplete_type.h"

TINT_INSTANTIATE_TYPEINFO(tint::resolver::IncompleteType);

namespace tint::resolver {

IncompleteType::IncompleteType(core::BuiltinType b)
    : Base(static_cast<size_t>(tint::TypeInfo::Of<IncompleteType>().full_hashcode),
           core::type::Flags{}),
      builtin(b) {}

IncompleteType::~IncompleteType() = default;

std::string IncompleteType::FriendlyName() const {
    return "<incomplete-type>";
}

uint32_t IncompleteType::Size() const {
    return 0;
}

uint32_t IncompleteType::Align() const {
    return 0;
}

core::type::Type* IncompleteType::Clone(core::type::CloneContext&) const {
    TINT_ICE() << "IncompleteType does not support cloning";
    return nullptr;
}

core::type::TypeAndCount IncompleteType::Elements(const Type*, uint32_t) const {
    return {};
}

const core::type::Type* IncompleteType::Element(uint32_t) const {
    return nullptr;
}

bool IncompleteType::Equals(const core::type::UniqueNode& other) const {
    if (auto* o = other.As<IncompleteType>()) {
        return o->builtin == builtin;
    }
    return false;
}

}  // namespace tint::resolver
