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

#include "tint/lang/glsl/validate/validate.h"

#include <string>

#include "glslang/Public/ResourceLimits.h"
#include "glslang/Public/ShaderLang.h"
#include "tint/utils/macros/static_init.h"
#include "tint/utils/text/string_stream.h"

namespace tint::glsl::validate {

namespace {

EShLanguage PipelineStageToEshLanguage(tint::ast::PipelineStage stage) {
    switch (stage) {
        case tint::ast::PipelineStage::kFragment:
            return EShLangFragment;
        case tint::ast::PipelineStage::kVertex:
            return EShLangVertex;
        case tint::ast::PipelineStage::kCompute:
            return EShLangCompute;
        default:
            TINT_UNREACHABLE();
            return EShLangVertex;
    }
}

}  // namespace

Result<SuccessType> Validate(const std::string& source, const EntryPointList& entry_points) {
    TINT_STATIC_INIT(glslang::InitializeProcess());

    for (auto entry_pt : entry_points) {
        EShLanguage lang = PipelineStageToEshLanguage(entry_pt.second);
        glslang::TShader shader(lang);
        const char* strings[1] = {source.c_str()};
        int lengths[1] = {static_cast<int>(source.length())};
        shader.setStringsWithLengths(strings, lengths, 1);
        shader.setEntryPoint("main");
        bool result =
            shader.parse(GetDefaultResources(), 310, EEsProfile, false, false, EShMsgDefault);
        if (!result) {
            StringStream err;
            err << "Error parsing GLSL shader:\n"
                << shader.getInfoLog() << "\n"
                << shader.getInfoDebugLog() << "\n";
            return Failure{err.str()};
        }
    }

    return Success;
}

}  // namespace tint::glsl::validate
