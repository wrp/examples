
use std::{
	env,
	process,
	io::BufReader,
	io::BufRead,
	fs::OpenOptions,
};


fn main() {
	for filename in env::args().skip(1).collect::<Vec<String>>() {
		let r = OpenOptions::new().read(true).open(&filename).unwrap_or_else(|err| {
			eprintln!("unable to open '{}': {}", filename, err);
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
