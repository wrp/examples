// Simple implementation of ls
use std::{fs, io};


fn main() -> io::Result<()> {
	let entries = fs::read_dir(".")?
		.map(|res| res.map(|e| e.path()))
		.collect::<Result<Vec<_>, io::Error>>()?;

	for e in entries.iter() {
		println!("{}", e.to_string_lossy());
	}

	Ok(())
}
