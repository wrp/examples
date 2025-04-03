
use std::io::{self, BufReader, BufRead};

// Use read_until to parse comma separated input

fn main() -> io::Result<()> {
	let mut buf = vec![];
	let mut reader = BufReader::new(io::stdin());
	let num_bytes = reader.read_until(b',', &mut buf)
		.expect("reading from cursor won't fail");
	println!("{}", num_bytes);
	println!("{:?}", buf);
	// On input "ab,...", num_bytes is 3 and buf[0] == 'a', buf[1] == 'b', buf[2] == ','
	Ok(())
}
