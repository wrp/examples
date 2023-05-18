
use std::{io,mem};

fn print_type_size<T>(x: &T) {
	println!("size of {} is {}", std::any::type_name::<T>(), mem::size_of_val(x));
}

fn main() -> io::Result<()> {
	let i = 32;
	print_type_size(&i);
	print_type_size(&2);
	print_type_size(&32_u64);
	Ok(())
}
