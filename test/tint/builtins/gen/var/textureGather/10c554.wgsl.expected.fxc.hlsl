TextureCube arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);

void textureGather_10c554() {
  float3 arg_2 = (0.0f).xxx;
  float4 res = arg_0.Gather(arg_1, arg_2);
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  textureGather_10c554();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  textureGather_10c554();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureGather_10c554();
  return;
}
