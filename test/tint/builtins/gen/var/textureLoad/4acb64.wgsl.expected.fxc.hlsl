Texture2DArray<float4> arg_0 : register(t0, space1);

void textureLoad_4acb64() {
  int2 arg_1 = (0).xx;
  uint arg_2 = 1u;
  int arg_3 = 1;
  float4 res = arg_0.Load(int4(int3(arg_1, int(arg_2)), arg_3));
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  textureLoad_4acb64();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  textureLoad_4acb64();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureLoad_4acb64();
  return;
}
