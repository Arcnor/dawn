#version 310 es

uniform highp sampler2DShadow arg_0_arg_1;

void textureSampleLevel_f3b2c8() {
  float res = textureLodOffset(arg_0_arg_1, vec3(0.0f, 0.0f, 0.0f), float(1u), ivec2(0));
}

vec4 vertex_main() {
  textureSampleLevel_f3b2c8();
  return vec4(0.0f);
}

void main() {
  gl_PointSize = 1.0;
  vec4 inner_result = vertex_main();
  gl_Position = inner_result;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  return;
}
#version 310 es
precision mediump float;

uniform highp sampler2DShadow arg_0_arg_1;

void textureSampleLevel_f3b2c8() {
  float res = textureLodOffset(arg_0_arg_1, vec3(0.0f, 0.0f, 0.0f), float(1u), ivec2(0));
}

void fragment_main() {
  textureSampleLevel_f3b2c8();
}

void main() {
  fragment_main();
  return;
}
#version 310 es

uniform highp sampler2DShadow arg_0_arg_1;

void textureSampleLevel_f3b2c8() {
  float res = textureLodOffset(arg_0_arg_1, vec3(0.0f, 0.0f, 0.0f), float(1u), ivec2(0));
}

void compute_main() {
  textureSampleLevel_f3b2c8();
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  compute_main();
  return;
}
