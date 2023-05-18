
// Display environment
use std::{env, io};

fn main() -> io::Result<()> {
	for (k,v) in env::vars() {
		println!("{} = {}", k, v);
	}

	Ok(())
}
