
use std::{
	env,
	fs::File,
	io::{prelude::*, BufReader},
	path::Path,
};

fn lines_from_file(filename: impl AsRef<Path>) -> Vec<String> {
	let file = File::open(filename).expect("learn how to handle this cleanly");
	let buf = BufReader::new(file);
	buf.lines()
		.map(|l| l.expect("Could not read line"))
		.collect()
}

// ---

fn main() {
	let args: Vec<String> = env::args().collect();
	let mut lines = lines_from_file(&args[1]);
	lines.sort();
	for line in lines {
		println!("{}", line);
	}
}
