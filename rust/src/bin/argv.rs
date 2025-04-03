// keywords: argv

use std::env;

fn main() {
	let argv: Vec<String> = env::args().collect();

	// The first argument is the path that was used to call the program.
	println!("My path is {}.", argv[0]);

	// The rest of the arguments are the passed command line parameters.
	println!("I got {} arguments: {:?}.", argv.len() - 1, &argv[1..]);

	// Get integer argument from first 2 arguments, defaulting to 5
	let c1 = argv.get(1).unwrap_or(&5.to_string()).parse::<u32>().unwrap_or(5);
	let c2 = match argv.get(2) { Some(d) => d.parse::<i32>().unwrap_or(5), None => 5 };
	println!("args = {}, {}", c1, c2);
}
