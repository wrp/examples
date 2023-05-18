
use std::{
	env,
	process,
	io::BufReader,
	io::BufRead,
	fs::OpenOptions,
};

fn open_file(path: &str) -> std::fs::File {
	OpenOptions::new()
		.read(true)
		.open(path)
		.unwrap_or_else(|err| {
			eprintln!("unable to open '{}': {}", path, err);
			process::exit(1);
			}
		)
}

fn main() {
	for filename in env::args().skip(1).collect::<Vec<String>>() {
		let r = BufReader::new(open_file(&filename));
		println!("succesfully opened {}", filename);
		println!("contents:");
		for line in r.lines() {
			println!("{}", line.unwrap());
		}
	}
}
