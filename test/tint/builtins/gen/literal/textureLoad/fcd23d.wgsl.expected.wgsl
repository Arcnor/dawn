@group(1) @binding(0) var arg_0 : texture_depth_multisampled_2d;

fn textureLoad_fcd23d() {
  var res : f32 = textureLoad(arg_0, vec2<u32>(), 1i);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureLoad_fcd23d();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureLoad_fcd23d();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureLoad_fcd23d();
}
