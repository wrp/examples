
use std::{env, fs::File, process};

fn main() {
	for filename in env::args().skip(1).collect::<Vec<String>>() {
		let mut _reader = File::open(&filename).unwrap_or_else(|err| {
			println!("unable to open '{}': {}", filename, err);
			process::exit(1);
		});
		println!("succesfully opened {}", filename);
	}
}
