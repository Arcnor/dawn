@group(1) @binding(0) var t : texture_2d<u32>;

@group(1) @binding(1) var s : sampler;

@stage(fragment)
fn main() {
  var res : vec4<u32> = textureGather(3, t, s, vec2<f32>());
}
