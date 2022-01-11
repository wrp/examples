
use std::{
	env,
	fs::File,
	io::{prelude::*, BufReader},
	path::Path,
};

fn lines_from_file(path: impl AsRef<Path> + std::fmt::Display) -> Vec<String> {


	// Open the path in read-only mode, returns `io::Result<File>`
	let file = match File::open(&path) {
		Err(why) => panic!("couldn't open {}: {}", path, why),
		Ok(file) => file,
	};
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
