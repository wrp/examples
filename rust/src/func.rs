// Some trivial examples


fn return_tuple(a: u32) -> (u32, u32) {
	return (a, a + 5);
}

fn main() {
	let (a, b) = return_tuple(3);

	println!("returned ({}, {})", a, b);
}

