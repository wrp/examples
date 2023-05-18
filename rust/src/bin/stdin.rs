// Read lines of input

use std::io::{self, BufRead};

fn main() -> io::Result<()> {
	let stdin = io::stdin();
	let mut line = String::new();

	// Read one line, demonstrating one technique
	let line1 = stdin.lock().lines().next().unwrap().unwrap();
	// line1 does *not* have a trailing newline
	println!("first line: {}", line1);

	// Read one line, demonstrating another technique
	// This *does* retain the trailing newline
	stdin.lock().read_line(&mut line).unwrap();
	print!("2nd line: {}", line);

	let mut lines = stdin.lock().lines();
	while let Some(line) = lines.next() {
		println!("{}", line.unwrap());
	}

	Ok(())
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
