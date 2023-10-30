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
//   src/tint/lang/core/interpolation_sampling.h.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_TINT_LANG_CORE_INTERPOLATION_SAMPLING_H_
#define SRC_TINT_LANG_CORE_INTERPOLATION_SAMPLING_H_

#include <cstdint>
#include <string>

#include "tint/utils/traits/traits.h"

namespace tint::core {

/// The interpolation sampling.
enum class InterpolationSampling : uint8_t {
    kUndefined,
    kCenter,
    kCentroid,
    kSample,
};

/// @param value the enum value
/// @returns the string for the given enum value
std::string_view ToString(InterpolationSampling value);

/// @param out the stream to write to
/// @param value the InterpolationSampling
/// @returns @p out so calls can be chained
template <typename STREAM, typename = traits::EnableIfIsOStream<STREAM>>
auto& operator<<(STREAM& out, InterpolationSampling value) {
    return out << ToString(value);
}

/// ParseInterpolationSampling parses a InterpolationSampling from a string.
/// @param str the string to parse
/// @returns the parsed enum, or InterpolationSampling::kUndefined if the string could not be
/// parsed.
InterpolationSampling ParseInterpolationSampling(std::string_view str);

constexpr const char* kInterpolationSamplingStrings[] = {
    "center",
    "centroid",
    "sample",
};

}  // namespace tint::core

#endif  // SRC_TINT_LANG_CORE_INTERPOLATION_SAMPLING_H_
