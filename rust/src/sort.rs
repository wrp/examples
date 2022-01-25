// Todo: look at std::fs::OpenOptions;

use std::{
	env,
	error::Error,
	fs::File,
	io::{prelude::*, BufReader},
	path::Path,
};

fn lines_from_file(path: impl AsRef<Path> + std::fmt::Display) -> Result<Vec<String>, std::io::Error> {


	// Open the path in read-only mode, returns `io::Result<File>`
	let file = File::open(&path)?;

	let buf = BufReader::new(file);

	Ok(buf.lines()
		.map(|l| l.expect("Could not read line"))
		.collect()
	)
}

// ---

fn run_app() -> Result<(), std::io::Error> {
	let args: Vec<String> = env::args().collect();
	let mut lines = lines_from_file(&args[1])?;
	lines.sort();
	for line in lines {
		println!("{}", line);
	}

	Ok(())
}

// boiler plate from https://doc.rust-lang.org/std/process/fn.exit.html
fn main() -> Result<(), Box<dyn Error>> {
	run_app()?;
	Ok(())
}
