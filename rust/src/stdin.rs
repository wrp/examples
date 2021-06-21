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
