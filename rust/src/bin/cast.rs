// From https://stevedonovan.github.io/rust-gentle-intro/1-basics.html


// cast an int as float
fn main() {
	let mut sum = 0.0;
	for i in 0..5 {
		sum += i as f64;
	}
	println!("sum is {0:.3}", sum as f64);

	// let a = 5.3f64;
	let b = 5i8;
	println!("f32 from i16: {}", f32::from(i16::from(b)))
}

// For precision specification: https://doc.rust-lang.org/std/fmt/index.html#syntax
