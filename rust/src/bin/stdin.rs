// Read lines of input

use std::{ io::{self, BufRead, prelude::* } };

fn main() -> io::Result<()> {
	let mut stdin = io::stdin();
	let mut line = String::new();

	// Read one line, demonstrating one technique
	let line1 = stdin.lock().lines().next().unwrap().unwrap();
	// line1 does *not* have a trailing newline
	println!("first line: {}", line1);

	// Read one line, demonstrating another technique
	// This *does* retain the trailing newline
	stdin.lock().read_line(&mut line).unwrap();
	print!("2nd line: {}", line);

	// Read one line, one char at a time
	print!("3rd line: (char at a time): '");
	let mut c;
	while { c = getchar(&mut stdin, 1); c[0] != '\n' as u8 } {
		print!("{}", c[0] as char);
	}
	println!("'");


	// Read the remainder, by line
	let mut lines = stdin.lock().lines();
	while let Some(line) = lines.next() {
		println!("{}", line.unwrap());
	}

	Ok(())
}

fn getchar<R>(reader: R, bytes_to_read: u64) -> Vec<u8>
where
	R: Read,
{
	let mut buf = vec![];
	let mut chunk = reader.take(bytes_to_read);
	let n = chunk.read_to_end(&mut buf).expect("Unexpected EOF");
	assert_eq!(bytes_to_read as usize, n);
	buf
}



// Read newline separated integers
//
// use std::{
//     io::{self, Read},
//     str::FromStr,
// };
//
// fn main() -> io::Result<()> {
// 	let mut string = String::new();
// 	io::stdin().read_to_string(&mut string)?;
// 	let result = string
// 		.lines()
// 		.map(i32::from_str)
// 		.collect::<Result<Vec<_>, _>>();
// 	println!("{:?}", result);
// 	Ok(())
// }
