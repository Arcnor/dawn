SKIP: FAILED

vector<float16_t, 4> tint_bitcast_to_f16(float2 src) {
  uint2 v = asuint(src);
  float2 t_low = f16tof32(v & 0xffff);
  float2 t_high = f16tof32((v >> 16) & 0xffff);
  return vector<float16_t, 4>(t_low.x, t_high.x, t_low.y, t_high.y);
}

[numthreads(1, 1, 1)]
void f() {
  const float2 a = float2(2.003662109375f, -513.03125f);
  const vector<float16_t, 4> b = tint_bitcast_to_f16(a);
  return;
}
FXC validation failure:
D:\Projects\RampUp\dawn\test\tint\expressions\bitcast\Shader@0x000001B7D61710A0(1,8-16): error X3000: syntax error: unexpected token 'float16_t'

