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

// GEN_BUILD:CONDITION(is_mac)

#import <Metal/Metal.h>

#include "tint/lang/msl/validate/val.h"

namespace tint::msl::validate {

Result UsingMetalAPI(const std::string& src, MslVersion version) {
    Result result;

    NSError* error = nil;

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device) {
        result.output = "MTLCreateSystemDefaultDevice returned null";
        result.failed = true;
        return result;
    }

    NSString* source = [NSString stringWithCString:src.c_str() encoding:NSUTF8StringEncoding];

    MTLCompileOptions* compileOptions = [MTLCompileOptions new];
    compileOptions.fastMathEnabled = true;
    switch (version) {
        case MslVersion::kMsl_1_2:
            compileOptions.languageVersion = MTLLanguageVersion1_2;
            break;
        case MslVersion::kMsl_2_1:
            compileOptions.languageVersion = MTLLanguageVersion2_1;
            break;
        case MslVersion::kMsl_2_3:
            if (@available(macOS 11.0, *)) {
                compileOptions.languageVersion = MTLLanguageVersion2_3;
            }
            break;
    }

    id<MTLLibrary> library = [device newLibraryWithSource:source
                                                  options:compileOptions
                                                    error:&error];
    if (!library) {
        NSString* output = [error localizedDescription];
        result.output = [output UTF8String];
        result.failed = true;
    }

    return result;
}

}  // namespace tint::msl::validate
