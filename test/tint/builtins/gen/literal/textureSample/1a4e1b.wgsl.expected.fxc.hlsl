Texture2DArray arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);

void textureSample_1a4e1b() {
  float res = arg_0.Sample(arg_1, float3(0.0f, 0.0f, float(1u))).x;
}

void fragment_main() {
  textureSample_1a4e1b();
  return;
}
