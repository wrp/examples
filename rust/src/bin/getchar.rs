// Demonstrate reading n bytes from a reader

#![allow(non_camel_case_types)]
#[allow(unused_imports)]

use std::{
	io::{self, prelude::*, BufReader},
	str,
};

fn getchar<R>(reader: R, bytes_to_read: u64) -> Vec<u8>
where
	R: Read,
{
	let mut buf = vec![];
	let mut chunk = reader.take(bytes_to_read);
	chunk.read_to_end(&mut buf).expect("Read Error");
	buf
}


fn main() {
	// let input_data = b"hello world";
	// let mut reader = BufReader::new(&input_data[..]);
	let mut reader = io::stdin();

	let first = getchar(&mut reader, 5);
	let _ = getchar2(&mut reader);
	let second = getchar(&mut reader, 5);

	println!(
		"{}, {}",
		str::from_utf8(&first).unwrap(),
		str::from_utf8(&second).unwrap()
	);
}

fn getchar2<R>(mut reader: R) -> u8
where
	R: Read,
{
	let mut b = vec![0_u8];
	_ = reader.read_exact(&mut b).expect("Read Error");
	b[0]
}
