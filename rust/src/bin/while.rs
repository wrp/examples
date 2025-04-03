
#![allow(non_camel_case_types)]
use std::{
	io,
};


fn main() -> io::Result<()> {
	let mut x = 1;

	// A basic while loop is of the form
	while x < 2 {
		println!("x = {}", x);
		x += 1;
	}

	// But the condition can be an arbitrary block, so you can also do
	while { x += 1; x < 5 } {
		println!("x = {}", x);
	}


	Ok(())
}
