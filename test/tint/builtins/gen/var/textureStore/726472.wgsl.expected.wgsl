@group(1) @binding(0) var arg_0 : texture_storage_2d<rg32float, write>;

fn textureStore_726472() {
  var arg_1 = vec2<u32>();
  var arg_2 = vec4<f32>();
  textureStore(arg_0, arg_1, arg_2);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureStore_726472();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureStore_726472();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureStore_726472();
}
