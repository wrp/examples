
use std::{
	collections::HashMap,
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
		for line in r.lines(){
			let mut words = HashMap::new();
			let s = line.expect("read_error");
			for word in s.split_whitespace(){
				let count = words.entry(word).or_insert(0);
				*count += 1;
			}
			println!("{:?}", words);
		}
	}
}


