@group(1) @binding(1) var arg_1 : texture_2d_array<u32>;

@group(1) @binding(2) var arg_2 : sampler;

fn textureGather_a0372b() {
  const arg_0 = 1;
  var arg_3 = vec2<f32>();
  var arg_4 = 1u;
  var res : vec4<u32> = textureGather(arg_0, arg_1, arg_2, arg_3, arg_4);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureGather_a0372b();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureGather_a0372b();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureGather_a0372b();
}
