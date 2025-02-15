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

#include "tint/lang/core/type/pointer.h"

#include "tint/lang/core/type/manager.h"
#include "tint/lang/core/type/reference.h"
#include "tint/utils/diagnostic/diagnostic.h"
#include "tint/utils/ice/ice.h"
#include "tint/utils/math/hash.h"
#include "tint/utils/text/string_stream.h"

TINT_INSTANTIATE_TYPEINFO(tint::core::type::Pointer);

namespace tint::core::type {

Pointer::Pointer(core::AddressSpace address_space, const Type* subtype, core::Access access)
    : Base(Hash(tint::TypeInfo::Of<Pointer>().full_hashcode, address_space, subtype, access),
           core::type::Flags{}),
      subtype_(subtype),
      address_space_(address_space),
      access_(access) {
    TINT_ASSERT(!subtype->Is<Reference>());
    TINT_ASSERT(access != core::Access::kUndefined);
}

bool Pointer::Equals(const UniqueNode& other) const {
    if (auto* o = other.As<Pointer>()) {
        return o->address_space_ == address_space_ && o->subtype_ == subtype_ &&
               o->access_ == access_;
    }
    return false;
}

std::string Pointer::FriendlyName() const {
    StringStream out;
    out << "ptr<";
    if (address_space_ != core::AddressSpace::kUndefined) {
        out << address_space_ << ", ";
    }
    out << subtype_->FriendlyName() << ", " << access_;
    out << ">";
    return out.str();
}

Pointer::~Pointer() = default;

Pointer* Pointer::Clone(CloneContext& ctx) const {
    auto* ty = subtype_->Clone(ctx);
    return ctx.dst.mgr->Get<Pointer>(address_space_, ty, access_);
}

}  // namespace tint::core::type
