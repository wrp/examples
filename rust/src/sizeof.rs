



use std::{io,mem};

fn main() -> io::Result<()> {
	let i = 32;
	println!("size of i is {}", mem::size_of_val(&i));
	Ok(())
}
