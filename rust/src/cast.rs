// From https://stevedonovan.github.io/rust-gentle-intro/1-basics.html

// cast an int as float

fn main() {
	let mut sum = 0.0;
	for i in 0..5 {
		sum += i as f64;
	}
	println!("sum is {0:.3}", sum as f64);
}

// For precision specification: https://doc.rust-lang.org/std/fmt/index.html#syntax
