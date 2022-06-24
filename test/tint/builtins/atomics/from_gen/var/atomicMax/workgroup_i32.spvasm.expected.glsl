#version 310 es

uint local_invocation_index_1 = 0u;
shared int arg_0;
void atomicMax_a89cc3() {
  int arg_1 = 0;
  int res = 0;
  arg_1 = 1;
  int x_15 = atomicMax(arg_0, arg_1);
  res = x_15;
  return;
}

void compute_main_inner(uint local_invocation_index) {
  atomicExchange(arg_0, 0);
  barrier();
  atomicMax_a89cc3();
  return;
}

void compute_main_1() {
  compute_main_inner(local_invocation_index_1);
  return;
}

void compute_main(uint local_invocation_index_1_param) {
  {
    atomicExchange(arg_0, 0);
  }
  barrier();
  local_invocation_index_1 = local_invocation_index_1_param;
  compute_main_1();
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  compute_main(gl_LocalInvocationIndex);
  return;
}
