static float4 x_GLF_color = float4(0.0f, 0.0f, 0.0f, 0.0f);
cbuffer cbuffer_x_5 : register(b0, space0) {
  uint4 x_5[2];
};
cbuffer cbuffer_x_8 : register(b1, space0) {
  uint4 x_8[2];
};

void main_1() {
  float a = 0.0f;
  const uint scalar_offset = ((16u * uint(0))) / 4;
  const int x_10 = asint(x_5[scalar_offset / 4][scalar_offset % 4]);
  const int x_11 = asint(x_5[1].x);
  const int x_12 = asint(x_5[1].x);
  const uint scalar_offset_1 = ((16u * uint(0))) / 4;
  const int x_13 = asint(x_5[scalar_offset_1 / 4][scalar_offset_1 % 4]);
  x_GLF_color = float4(float(x_10), float(x_11), float(x_12), float(x_13));
  const float x_45 = asfloat(x_8[1].x);
  a = (asfloat(0xff800000u) % x_45);
  const float x_47 = a;
  const uint scalar_offset_2 = ((16u * uint(0))) / 4;
  const float x_49 = asfloat(x_8[scalar_offset_2 / 4][scalar_offset_2 % 4]);
  if ((x_47 != x_49)) {
    const uint scalar_offset_3 = ((16u * uint(0))) / 4;
    const float x_54 = asfloat(x_8[scalar_offset_3 / 4][scalar_offset_3 % 4]);
    x_GLF_color.y = x_54;
  }
  return;
}

struct main_out {
  float4 x_GLF_color_1;
};
struct tint_symbol {
  float4 x_GLF_color_1 : SV_Target0;
};

tint_symbol main() {
  main_1();
  const main_out tint_symbol_1 = {x_GLF_color};
  const tint_symbol tint_symbol_4 = {tint_symbol_1.x_GLF_color_1};
  return tint_symbol_4;
}
