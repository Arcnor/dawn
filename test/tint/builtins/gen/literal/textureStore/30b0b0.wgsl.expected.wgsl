@group(1) @binding(0) var arg_0 : texture_storage_2d_array<rgba32sint, write>;

fn textureStore_30b0b0() {
  textureStore(arg_0, vec2<i32>(), 1u, vec4<i32>());
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureStore_30b0b0();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureStore_30b0b0();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureStore_30b0b0();
}
