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
//   src/tint/lang/core/attribute_bench.cc.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "tint/lang/core/attribute.h"

#include <array>

#include "benchmark/benchmark.h"

namespace tint::core {
namespace {

void AttributeParser(::benchmark::State& state) {
    const char* kStrings[] = {
        "alccn",
        "3g",
        "aVign",
        "align",
        "alig1",
        "qqlJn",
        "alill7n",
        "ppqqndnHH",
        "bicv",
        "bndiGg",
        "binding",
        "bindiivg",
        "8WWnding",
        "bxxding",
        "bXltigg",
        "ultXn",
        "buil3in",
        "builtin",
        "Euiltin",
        "bPTTltin",
        "builtdxx",
        "c44mpute",
        "coSSpuVVe",
        "RomR22e",
        "compute",
        "cFpu9e",
        "comute",
        "VOORRHte",
        "dyagnstic",
        "d77agnnnsllrrc",
        "dia400ostic",
        "diagnostic",
        "danstooc",
        "dignszzic",
        "d11ansppiic",
        "XXragment",
        "fIIa9955nnnt",
        "aarHHgmenYSS",
        "fragment",
        "fkkaet",
        "gjamRRn",
        "fabmnt",
        "gjoup",
        "goup",
        "goq",
        "group",
        "Nroup",
        "govv",
        "gruQQ",
        "r",
        "jd",
        "NNw",
        "id",
        "i",
        "rrd",
        "iG",
        "inFFex",
        "iE",
        "inrrx",
        "index",
        "inx",
        "inJJD",
        "ie",
        "inerpklae",
        "intrpolate",
        "inJerpolae",
        "interpolate",
        "interpocate",
        "interpolaOe",
        "__nttevvpoKKate",
        "xnvari5n8",
        "inFq__ant",
        "iqqariant",
        "invariant",
        "invar6a33O",
        "i96arQttanoo",
        "inari66nt",
        "lOxati6zz",
        "locyytion",
        "lHHtion",
        "location",
        "qWW4caton",
        "locOOton",
        "ocatiYn",
        "m_use",
        "mutFuse",
        "wust_us",
        "must_use",
        "Kst_sff",
        "qusKK_use",
        "mFsmm_3se",
        "ize",
        "sze",
        "sbbb",
        "size",
        "iie",
        "siqe",
        "svvTTe",
        "vertFFx",
        "vrQ00P",
        "vePtex",
        "vertex",
        "vsste77",
        "veCtRRbb",
        "verteXX",
        "workgqou_siCCOOO",
        "worsgroupsuzL",
        "wXrkgroup_size",
        "workgroup_size",
        "workgroup_sze",
        "wqqrOgoupize",
        "workg22oup_size",
    };
    for (auto _ : state) {
        for (auto* str : kStrings) {
            auto result = ParseAttribute(str);
            benchmark::DoNotOptimize(result);
        }
    }
}  // NOLINT(readability/fn_size)

BENCHMARK(AttributeParser);

}  // namespace
}  // namespace tint::core
