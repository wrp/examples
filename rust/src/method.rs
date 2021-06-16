// Simple example of an implementation block defining a method
// From the book: ch05-03-method-syntax.html

use std::env;

#[derive(Debug)]
struct Rectangle {
	width: u32,
	height: u32,
}

impl Rectangle {
	fn area(&self) -> u32 {
		self.width * self.height
	}
}

fn main() {
	let args: Vec<String> = env::args().collect();
	let width = args[1].parse::<u32>().unwrap();
	let height = args[2].parse::<u32>().unwrap();

	let rect1 = Rectangle {
		width,
		height,
	};

	println!("The area of the rectangle is {} square pixels.", rect1.area());
}

