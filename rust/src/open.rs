
use std::{env, fs::File, process, io::BufReader, io::BufRead};

fn main() {
	for filename in env::args().skip(1).collect::<Vec<String>>() {
		let r = File::open(&filename).unwrap_or_else(|err| {
			println!("unable to open '{}': {}", filename, err);
			process::exit(1);
		});
		let r = BufReader::new(r);
		println!("succesfully opened {}", filename);
		println!("contents:");
		for line in r.lines() {
			println!("{}", line.unwrap());
		}
	}
}
