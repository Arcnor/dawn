// Copyright 2021 The Tint Authors.
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
// File generated by tools/intrinsic-gen
// using the template:
//   test/intrinsics/intrinsics.wgsl.tmpl
// and the intrinsic defintion file:
//   src/intrinsics.def
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////


// fn tanh(vec<4, f32>) -> vec<4, f32>
fn tanh_5663c5() {
  var res: vec4<f32> = tanh(vec4<f32>());
}

@stage(vertex)
fn vertex_main() -> @builtin(position) vec4<f32> {
  tanh_5663c5();
  return vec4<f32>();
}

@stage(fragment)
fn fragment_main() {
  tanh_5663c5();
}

@stage(compute) @workgroup_size(1)
fn compute_main() {
  tanh_5663c5();
}
