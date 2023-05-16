// Read lines of input

use std::io::{self, BufRead};

fn main() -> io::Result<()> {
	let stdin = io::stdin();
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
