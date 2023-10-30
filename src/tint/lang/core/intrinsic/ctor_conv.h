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

////////////////////////////////////////////////////////////////////////////////
// File generated by 'tools/src/cmd/gen' using the template:
//   src/tint/lang/core/intrinsic/ctor_conv.h.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_TINT_LANG_CORE_INTRINSIC_CTOR_CONV_H_
#define SRC_TINT_LANG_CORE_INTRINSIC_CTOR_CONV_H_

#include <cstdint>

#include "tint/utils/traits/traits.h"

namespace tint::core::intrinsic {

/// CtorConv is an enumerator of types that have a constructor or converter overload
/// declared in the intrinsic table.
enum class CtorConv : uint8_t {
    kI32,
    kU32,
    kF32,
    kF16,
    kBool,
    kVec2,
    kVec3,
    kVec4,
    kMat2x2,
    kMat2x3,
    kMat2x4,
    kMat3x2,
    kMat3x3,
    kMat3x4,
    kMat4x2,
    kMat4x3,
    kMat4x4,
    kPackedVec3,
    kNone,
};

/// @returns the name of the enumerator
/// @param i the CtorConv enumerator
const char* str(CtorConv i);

/// Prints the CtorConv @p c to @p o
/// @param o the stream to write to
/// @param c the CtorConv
/// @return the stream so calls can be chained
template <typename STREAM, typename = traits::EnableIfIsOStream<STREAM>>
auto& operator<<(STREAM& o, CtorConv c) {
    return o << str(c);
}

/// @param n the width of the vector
/// @return the CtorConv for a vector of width `n`
inline CtorConv VectorCtorConv(uint32_t n) {
    switch (n) {
        case 2:
            return CtorConv::kVec2;
        case 3:
            return CtorConv::kVec3;
        case 4:
            return CtorConv::kVec4;
    }
    return CtorConv::kNone;
}

/// @param c the number of columns in the matrix
/// @param r the number of rows in the matrix
/// @return the CtorConv for a matrix with `c` columns and `r` rows
inline CtorConv MatrixCtorConv(uint32_t c, uint32_t r) {
    switch ((c - 2) * 3 + (r - 2)) {
        case 0:
            return CtorConv::kMat2x2;
        case 1:
            return CtorConv::kMat2x3;
        case 2:
            return CtorConv::kMat2x4;
        case 3:
            return CtorConv::kMat3x2;
        case 4:
            return CtorConv::kMat3x3;
        case 5:
            return CtorConv::kMat3x4;
        case 6:
            return CtorConv::kMat4x2;
        case 7:
            return CtorConv::kMat4x3;
        case 8:
            return CtorConv::kMat4x4;
    }
    return CtorConv::kNone;
}

}  // namespace tint::core::intrinsic

#endif  // SRC_TINT_LANG_CORE_INTRINSIC_CTOR_CONV_H_
