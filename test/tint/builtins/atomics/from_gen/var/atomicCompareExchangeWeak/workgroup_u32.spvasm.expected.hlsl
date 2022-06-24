struct atomic_compare_exchange_resultu32 {
  uint old_value;
  bool exchanged;
};
struct x__atomic_compare_exchange_resultu32 {
  uint old_value;
  bool exchanged;
};

static uint local_invocation_index_1 = 0u;
groupshared uint arg_0;

void atomicCompareExchangeWeak_83580d() {
  uint arg_1 = 0u;
  uint arg_2 = 0u;
  x__atomic_compare_exchange_resultu32 res = {0u, false};
  arg_1 = 1u;
  arg_2 = 1u;
  const uint x_21 = arg_2;
  atomic_compare_exchange_resultu32 atomic_result = (atomic_compare_exchange_resultu32)0;
  uint atomic_compare_value = arg_1;
  InterlockedCompareExchange(arg_0, atomic_compare_value, x_21, atomic_result.old_value);
  atomic_result.exchanged = atomic_result.old_value == atomic_compare_value;
  const atomic_compare_exchange_resultu32 tint_symbol_2 = atomic_result;
  const uint old_value_1 = tint_symbol_2.old_value;
  const uint x_23 = old_value_1;
  const x__atomic_compare_exchange_resultu32 tint_symbol_3 = {x_23, (x_23 == x_21)};
  res = tint_symbol_3;
  return;
}

void compute_main_inner(uint local_invocation_index) {
  uint atomic_result_1 = 0u;
  InterlockedExchange(arg_0, 0u, atomic_result_1);
  GroupMemoryBarrierWithGroupSync();
  atomicCompareExchangeWeak_83580d();
  return;
}

void compute_main_1() {
  compute_main_inner(local_invocation_index_1);
  return;
}

struct tint_symbol_1 {
  uint local_invocation_index_1_param : SV_GroupIndex;
};

void compute_main_inner_1(uint local_invocation_index_1_param) {
  {
    uint atomic_result_2 = 0u;
    InterlockedExchange(arg_0, 0u, atomic_result_2);
  }
  GroupMemoryBarrierWithGroupSync();
  local_invocation_index_1 = local_invocation_index_1_param;
  compute_main_1();
}

[numthreads(1, 1, 1)]
void compute_main(tint_symbol_1 tint_symbol) {
  compute_main_inner_1(tint_symbol.local_invocation_index_1_param);
  return;
}
