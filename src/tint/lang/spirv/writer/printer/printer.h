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

#ifndef SRC_TINT_LANG_SPIRV_WRITER_PRINTER_PRINTER_H_
#define SRC_TINT_LANG_SPIRV_WRITER_PRINTER_PRINTER_H_

#include <cstdint>
#include <vector>

#include "tint/lang/spirv/writer/common/module.h"
#include "tint/utils/result/result.h"

// Forward declarations
namespace tint::core::ir {
class Module;
}  // namespace tint::core::ir

namespace tint::spirv::writer {

/// @returns the generated SPIR-V instructions on success, or failure
/// @param module the Tint IR module to generate
/// @param zero_init_workgroup_memory `true` to initialize all the variables in the Workgroup
///                                   storage class with OpConstantNull
tint::Result<std::vector<uint32_t>> Print(core::ir::Module& module,
                                          bool zero_init_workgroup_memory);

/// @returns the generated SPIR-V module on success, or failure
/// @param module the Tint IR module to generate
/// @param zero_init_workgroup_memory `true` to initialize all the variables in the Workgroup
///                                   storage class with OpConstantNull
tint::Result<Module> PrintModule(core::ir::Module& module, bool zero_init_workgroup_memory);

}  // namespace tint::spirv::writer

#endif  // SRC_TINT_LANG_SPIRV_WRITER_PRINTER_PRINTER_H_
