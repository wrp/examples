// Simple example of an implementation block defining a method
// From the book: ch05-03-method-syntax.html

use std::env;
// use structopt::StructOpt;

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
	let width = if args.len() > 1 { args[1].parse::<u32>().unwrap()} else {5};
	let height = if args.len() > 2 { args[2].parse::<u32>().unwrap()} else {10};

	let rect1 = Rectangle {
		width,
		height,
	};

	println!("The area of the rectangle is {} square pixels.", rect1.area());
}

