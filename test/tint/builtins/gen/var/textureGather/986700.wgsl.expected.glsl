#version 310 es

uniform highp usampler2D arg_1_arg_2;

void textureGather_986700() {
  vec2 arg_3 = vec2(0.0f);
  uvec4 res = textureGatherOffset(arg_1_arg_2, arg_3, ivec2(0), int(1u));
}

vec4 vertex_main() {
  textureGather_986700();
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

uniform highp usampler2D arg_1_arg_2;

void textureGather_986700() {
  vec2 arg_3 = vec2(0.0f);
  uvec4 res = textureGatherOffset(arg_1_arg_2, arg_3, ivec2(0), int(1u));
}

void fragment_main() {
  textureGather_986700();
}

void main() {
  fragment_main();
  return;
}
#version 310 es

uniform highp usampler2D arg_1_arg_2;

void textureGather_986700() {
  vec2 arg_3 = vec2(0.0f);
  uvec4 res = textureGatherOffset(arg_1_arg_2, arg_3, ivec2(0), int(1u));
}

void compute_main() {
  textureGather_986700();
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  compute_main();
  return;
}
