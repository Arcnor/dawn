#version 310 es

void f() {
  vec3 b = vec3(2.003662109375f, -513.03125f, -1024.25f);
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  f();
  return;
}
