SKIP: FAILED

static uint var_1 = 0u;

void main_1() {
  {
    [loop] for(; !(false); ) {
    }
  }
  return;
}

void main() {
  main_1();
  return;
}
warning: DXIL.dll not found.  Resulting DXIL will not be signed for use in release environments.

error: validation errors
/tmp/tint_ePdsZG:11: error: Loop must have break.
Validation failed.



