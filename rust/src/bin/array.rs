
fn main() {
	let a: [i32; 5] = [1, 2, 3, 4, 5];
	let b = [1; 3];  // 1, 1, 1
	let c: [f32; 3] = [1.0, 2.2, 3.2];

	println!("a = {:?}", a);
	println!("a = {a:?}");
	println!("b = {:?}", b);
	println!("c = {:?}", c);
}
