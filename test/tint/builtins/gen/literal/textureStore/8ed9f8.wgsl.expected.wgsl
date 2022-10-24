@group(1) @binding(0) var arg_0 : texture_storage_3d<rgba8unorm, write>;

fn textureStore_8ed9f8() {
  textureStore(arg_0, vec3<u32>(), vec4<f32>());
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureStore_8ed9f8();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureStore_8ed9f8();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureStore_8ed9f8();
}
