@group(1) @binding(0) var arg_0 : texture_storage_2d_array<rgba8unorm, write>;

fn textureStore_330b7c() {
  textureStore(arg_0, vec2<u32>(), 1u, vec4<f32>());
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureStore_330b7c();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureStore_330b7c();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureStore_330b7c();
}
