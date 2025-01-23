
// Simple reverse polish (postfix) calculator

use std::{
	io,
	convert::TryInto
};

fn _chr(c: u8) -> char {
	<u8 as TryInto<char>>::try_into(c).unwrap()
}

fn ord(c: char) -> u32 {
	c.into()
	// <char as Into<i32>>::into(c)
}


fn main() {
	let mut lines = io::stdin().lines();
	let mut stack = Vec::<f32>::new();
	let mut accum: Option<f32> = None;

	while let Some(line) = lines.next() {
		for c in line.unwrap().chars() {
			if c.is_digit(10) {
				let val = (ord(c) - ord('0')) as f32;
				accum = match accum {
				None => Some(val),
				Some(x) => Some(10.0 * x + (ord(c) - ord('0')) as f32),
				}
			} else if accum.is_some(){
				stack.push(accum.unwrap());
				accum = None;
			}
			let (a, b) = match c {
				'+'|'-'|'*'|'/' => (stack.pop().unwrap(), stack.pop().unwrap()),
				_ => (0.0, 0.0)
			};
			match c {
			'+' => stack.push(a + b),
			'-' => stack.push(b - a),
			'*' => stack.push(a * b),
			'/' => stack.push(b / a),
			'p' => println!("{}", stack.pop().unwrap()),
			'q' => return,
			' ' | '0'..='9' => {},
			 _ => todo!(),
			}
		}
	}
	println!("");
}
