
#![allow(non_camel_case_types)]

use std::{
	// env,
	io::{self, Read},
	// fs::File,
	str::FromStr,
	// path::Path,
};


fn main() -> io::Result<()> {
	let mut string = String::new();
	io::stdin().read_to_string(&mut string)?;
	let mut sum = 0;
	for n in string
		.lines()
		.map(i32::from_str)
	{
		sum += n.unwrap() / 3 - 2;
	}
	println!("{:?}", sum);
	Ok(())
}
