@group(0) @binding(0) var t_f : texture_depth_2d_array;

@stage(compute) @workgroup_size(1)
fn main() {
  _ = t_f;
}
