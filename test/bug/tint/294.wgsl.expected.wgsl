struct Light {
  position : vec3<f32>;
  colour : vec3<f32>;
}

struct Lights {
  light : @stride(32) array<Light>;
}

@group(0) @binding(1) var<storage, read> lights : Lights;
