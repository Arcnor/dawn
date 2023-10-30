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

////////////////////////////////////////////////////////////////////////////////
// File generated by 'tools/src/cmd/gen' using the template:
//   src/tint/lang/core/builtin_value_bench.cc.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "tint/lang/core/builtin_value.h"

#include <array>

#include "benchmark/benchmark.h"

namespace tint::core {
namespace {

void BuiltinValueParser(::benchmark::State& state) {
    const char* kStrings[] = {
        "_ccpoint_siz",
        "_3poi_ile",
        "__poiVt_size",
        "__point_size",
        "1_point_size",
        "__pointJsqze",
        "__lloint_siz77",
        "frqqgppepHHh",
        "fv_dcpt",
        "frabGdeth",
        "frag_depth",
        "frag_veiith",
        "fr8g_depWWh",
        "Mragxxepth",
        "ggroXtfacing",
        "Vot_fuciXg",
        "front_fac3ng",
        "front_facing",
        "front_fEcing",
        "fronPPfaTTing",
        "ddroxxtfacing",
        "global_invocatio44_id",
        "global_invocaSSioVV_id",
        "22loRal_invoRtion_id",
        "global_invocation_id",
        "globalFinvoction_id",
        "gloal_invocation_id",
        "RRlHOOaV_invoction_id",
        "instance_ydex",
        "instGlr77cnn_index",
        "instan04e_index",
        "instance_index",
        "insacoo_inex",
        "izstane_index",
        "nippance_in11ex",
        "local_invXXcation_id",
        "lIIcal_i5599ocation_inn",
        "HHrrcal_inSSocation_Yaa",
        "local_invocation_id",
        "lokkal_invocatini",
        "jocal_invocRRongid",
        "local_inocatbon_i",
        "local_injocation_index",
        "local_invocatio_index",
        "locl_invocqtion_ndex",
        "local_invocation_index",
        "localNNinvocaton_index",
        "local_invocatin_ivvdx",
        "locl_invocatioQQ_index",
        "num_workgffus",
        "num_workgroujs",
        "num_wrkgNNwoup8",
        "num_workgroups",
        "numworkgroups",
        "num_workrrroups",
        "num_worGgroups",
        "pFFsition",
        "pEiio",
        "prrsitio",
        "position",
        "sition",
        "poJJDtin",
        "poi8i",
        "smpke11nde",
        "samle_index",
        "saple_Jndex",
        "sample_index",
        "cample_index",
        "sample_indOx",
        "savvKKl___inttex",
        "sam8le_xx5k",
        "sampqq__msk",
        "sampleqmask",
        "sample_mask",
        "33amOe_mas66",
        "samoott6QQmask",
        "66mple_mask",
        "subroup_invoOaxion_i6zz",
        "subgroup_inyyocation_id",
        "subgrup_invcatiZHH_id",
        "subgroup_invocation_id",
        "subgroqp_inWWoat44on_id",
        "subgrou_inOOocation_id",
        "suhgrup_invoYation_id",
        "subroup_si",
        "suFgoup_size",
        "subgowp_size",
        "subgroup_size",
        "suffgKup_sie",
        "KKubgroqp_size",
        "subFroup3mmize",
        "ertex_index",
        "verteq_inex",
        "verbx_indbbx",
        "vertex_index",
        "iertex_indx",
        "veOOtexqidex",
        "vertexvvindTTx",
        "workFFroup_id",
        "workgPfpQ00d",
        "woPkgroup_id",
        "workgroup_id",
        "wosskgrup_i77",
        "workgroup_bbRC",
        "workgroup_iXX",
    };
    for (auto _ : state) {
        for (auto* str : kStrings) {
            auto result = ParseBuiltinValue(str);
            benchmark::DoNotOptimize(result);
        }
    }
}  // NOLINT(readability/fn_size)

BENCHMARK(BuiltinValueParser);

}  // namespace
}  // namespace tint::core
