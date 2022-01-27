
use std::{io,mem};

fn print_type_of<T>(x: &T) {
	println!("size of {} is {}", std::any::type_name::<T>(), mem::size_of_val(x));
}

fn main() -> io::Result<()> {
	let i = 32;
	print_type_of(&i);
	Ok(())
}
