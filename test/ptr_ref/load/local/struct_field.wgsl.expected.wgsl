struct S {
  i : i32;
}

@stage(compute) @workgroup_size(1)
fn main() {
  var V : S;
  var i : i32 = V.i;
  return;
}
