
use std::{
	error::Error,
	env,
	process,
	io::{BufReader, BufRead},
	fs::{File, OpenOptions},
};

fn open_file(path: &str) -> std::fs::File {
	OpenOptions::new()
	.read(true)
	.open(path)
	.unwrap_or_else(
		|err| {
			eprintln!("{}: {}", path, err);
			process::exit(1);
		}
	)
}


fn last_char_of_first_line(text: BufReader<File>) -> Option<char> {
	text.lines().next()?.expect("FOO").chars().last()
}


fn main() -> Result<(), Box<dyn Error>> {
	for filename in env::args().skip(1).collect::<Vec<String>>() {
		let r = BufReader::new(open_file(&filename));
		println!("succesfully opened {}", filename);
		println!("contents:");
		for line in r.lines() {
			println!("{}", line.unwrap());
		}
	}

	// Demonstrate using ? operator (useless, since the path used
	// to open the file does not get into the error message)
	let greeting_file = BufReader::new(File::open("hello.txt")?);
	println!("Last char of first line is: {}",
		last_char_of_first_line(greeting_file).unwrap());

	Ok(())
}
