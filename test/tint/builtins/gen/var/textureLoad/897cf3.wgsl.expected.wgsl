@group(1) @binding(0) var arg_0 : texture_2d<u32>;

fn textureLoad_897cf3() {
  var arg_1 = vec2<u32>();
  var arg_2 = 1u;
  var res : vec4<u32> = textureLoad(arg_0, arg_1, arg_2);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureLoad_897cf3();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureLoad_897cf3();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureLoad_897cf3();
}
