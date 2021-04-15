// https://doc.rust-lang.org/stable/rust-by-example/conversion/from_into.html

use std::convert::From;

#[derive(Debug)]
struct Number {
	value: i32,
}

impl From<i32> for Number {
	fn from(item: i32) -> Self {
		Number { value: item }
	}
}

fn main() {
	let int = 5;
	let num: Number = int.into();
	println!("My number is {:?}", num);
}
