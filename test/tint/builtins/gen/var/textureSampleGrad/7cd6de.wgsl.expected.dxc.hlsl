Texture2DArray<float4> arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);

void textureSampleGrad_7cd6de() {
  float2 arg_2 = (0.0f).xx;
  uint arg_3 = 1u;
  float2 arg_4 = (0.0f).xx;
  float2 arg_5 = (0.0f).xx;
  float4 res = arg_0.SampleGrad(arg_1, float3(arg_2, float(arg_3)), arg_4, arg_5, (0).xx);
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  textureSampleGrad_7cd6de();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  textureSampleGrad_7cd6de();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureSampleGrad_7cd6de();
  return;
}
