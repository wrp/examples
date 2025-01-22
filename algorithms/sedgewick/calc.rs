
// Simple reverse polish (postfix) calculator
// Extremely fragile, but works
// (eg, input stream "334 2 +p" gives 336",
// but "334 2 + p" yields 0, since the space
// after the '+' pushes a 0 to the stack.)

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
	let mut accum: f32 = 0.0;

	while let Some(line) = lines.next() {
		for c in line.unwrap().chars() {
			match c {
			'+' => { let a = stack.pop().unwrap(); let b = stack.pop().unwrap(); stack.push(a + b) },
			'-' => { let a = stack.pop().unwrap(); let b = stack.pop().unwrap(); stack.push(a - b) },
			'*' => { let a = stack.pop().unwrap(); let b = stack.pop().unwrap(); stack.push(a * b) },
			'/' => { let a = stack.pop().unwrap(); let b = stack.pop().unwrap(); stack.push(a / b) },
			'0'..='9' => accum = 10.0 * accum + (ord(c) - ord('0')) as f32,
			' ' => { stack.push(accum); accum = 0.0; },
			'p' => print!("{}", stack.pop().unwrap()),
			 _ => todo!(),
			}
		}
	}
	println!("");
}
