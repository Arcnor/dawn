@group(1) @binding(0) var arg_0 : texture_storage_2d_array<rgba16uint, write>;

fn textureStore_33cec0() {
  textureStore(arg_0, vec2<u32>(), 1i, vec4<u32>());
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureStore_33cec0();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureStore_33cec0();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureStore_33cec0();
}
