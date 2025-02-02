#![allow(non_camel_case_types)]


// Read a stream of integers

extern crate text_io;
use text_io::try_read;
use std::io;

fn main() -> io::Result<()> {
	let mut a: Result<i32, _>;
	while {
		a = try_read!();
		a.is_ok()
	}
	{
		println!("a = {}", a.unwrap());
	}

	Ok(())
}
