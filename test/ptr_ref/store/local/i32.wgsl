@stage(compute) @workgroup_size(1)
fn main() {
  var i : i32 = 123;
  let p : ptr<function, i32> = &i;
  *p = 123; // constant
  *p = 100 + 20 + 3; // dynamic
}
