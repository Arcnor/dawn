Texture1D<float4> arg_0 : register(t0, space1);

void textureDimensions_16f7cb() {
  uint arg_1 = 1u;
  int2 tint_tmp;
  arg_0.GetDimensions(arg_1, tint_tmp.x, tint_tmp.y);
  int res = tint_tmp.x;
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  textureDimensions_16f7cb();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  textureDimensions_16f7cb();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureDimensions_16f7cb();
  return;
}
