@group(1) @binding(0) var arg_0 : texture_storage_3d<rgba16float, write>;

fn textureStore_8b9310() {
  var arg_1 = vec3<u32>();
  var arg_2 = vec4<f32>();
  textureStore(arg_0, arg_1, arg_2);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureStore_8b9310();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureStore_8b9310();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureStore_8b9310();
}
