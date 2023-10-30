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
//   src/tint/lang/spirv/builtin_fn.cc.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "tint/lang/spirv/builtin_fn.h"

namespace tint::spirv {

const char* str(BuiltinFn i) {
    switch (i) {
        case BuiltinFn::kNone:
            return "<none>";
        case BuiltinFn::kArrayLength:
            return "array_length";
        case BuiltinFn::kAtomicAnd:
            return "atomic_and";
        case BuiltinFn::kAtomicCompareExchange:
            return "atomic_compare_exchange";
        case BuiltinFn::kAtomicExchange:
            return "atomic_exchange";
        case BuiltinFn::kAtomicIadd:
            return "atomic_iadd";
        case BuiltinFn::kAtomicIsub:
            return "atomic_isub";
        case BuiltinFn::kAtomicLoad:
            return "atomic_load";
        case BuiltinFn::kAtomicOr:
            return "atomic_or";
        case BuiltinFn::kAtomicSmax:
            return "atomic_smax";
        case BuiltinFn::kAtomicSmin:
            return "atomic_smin";
        case BuiltinFn::kAtomicStore:
            return "atomic_store";
        case BuiltinFn::kAtomicUmax:
            return "atomic_umax";
        case BuiltinFn::kAtomicUmin:
            return "atomic_umin";
        case BuiltinFn::kAtomicXor:
            return "atomic_xor";
        case BuiltinFn::kDot:
            return "dot";
        case BuiltinFn::kImageDrefGather:
            return "image_dref_gather";
        case BuiltinFn::kImageFetch:
            return "image_fetch";
        case BuiltinFn::kImageGather:
            return "image_gather";
        case BuiltinFn::kImageQuerySize:
            return "image_query_size";
        case BuiltinFn::kImageQuerySizeLod:
            return "image_query_size_lod";
        case BuiltinFn::kImageRead:
            return "image_read";
        case BuiltinFn::kImageSampleImplicitLod:
            return "image_sample_implicit_lod";
        case BuiltinFn::kImageSampleExplicitLod:
            return "image_sample_explicit_lod";
        case BuiltinFn::kImageSampleDrefImplicitLod:
            return "image_sample_dref_implicit_lod";
        case BuiltinFn::kImageSampleDrefExplicitLod:
            return "image_sample_dref_explicit_lod";
        case BuiltinFn::kImageWrite:
            return "image_write";
        case BuiltinFn::kMatrixTimesMatrix:
            return "matrix_times_matrix";
        case BuiltinFn::kMatrixTimesScalar:
            return "matrix_times_scalar";
        case BuiltinFn::kMatrixTimesVector:
            return "matrix_times_vector";
        case BuiltinFn::kSampledImage:
            return "sampled_image";
        case BuiltinFn::kSelect:
            return "select";
        case BuiltinFn::kVectorTimesMatrix:
            return "vector_times_matrix";
        case BuiltinFn::kVectorTimesScalar:
            return "vector_times_scalar";
        case BuiltinFn::kSdot:
            return "sdot";
        case BuiltinFn::kUdot:
            return "udot";
    }
    return "<unknown>";
}

}  // namespace tint::spirv
