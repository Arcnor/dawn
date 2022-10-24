// Copyright 2022 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

////////////////////////////////////////////////////////////////////////////////
// File generated by tools/src/cmd/gen
// using the template:
//   test/tint/builtins/gen/gen.wgsl.tmpl
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

@group(1) @binding(0) var arg_0: texture_depth_2d_array;
@group(1) @binding(1) var arg_1: sampler_comparison;

// fn textureGatherCompare(texture: texture_depth_2d_array, sampler: sampler_comparison, coords: vec2<f32>, array_index: u32, depth_ref: f32, @const offset: vec2<i32>) -> vec4<f32>
fn textureGatherCompare_144a9a() {
  var arg_2 = vec2<f32>();
  var arg_3 = 1u;
  var arg_4 = 1.f;
  const arg_5 = vec2<i32>();
  var res: vec4<f32> = textureGatherCompare(arg_0, arg_1, arg_2, arg_3, arg_4, arg_5);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureGatherCompare_144a9a();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureGatherCompare_144a9a();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureGatherCompare_144a9a();
}
