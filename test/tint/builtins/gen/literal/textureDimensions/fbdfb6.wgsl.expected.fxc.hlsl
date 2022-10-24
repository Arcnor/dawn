Texture3D<float4> arg_0 : register(t0, space1);

void textureDimensions_fbdfb6() {
  int4 tint_tmp;
  arg_0.GetDimensions(1u, tint_tmp.x, tint_tmp.y, tint_tmp.z, tint_tmp.w);
  int3 res = tint_tmp.xyz;
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  textureDimensions_fbdfb6();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  textureDimensions_fbdfb6();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureDimensions_fbdfb6();
  return;
}
