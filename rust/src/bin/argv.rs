
use std::env;

fn main() {
	let args: Vec<String> = env::args().collect();

	// The first argument is the path that was used to call the program.
	println!("My path is {}.", args[0]);

	// The rest of the arguments are the passed command line parameters.
	// Call the program like this:
	//   $ ./args arg1 arg2
	println!("I got {:?} arguments: {:?}.", args.len() - 1, &args[1..]);

	// Get integer argument from first 2 args, defaulting to 5
	let c1 = args.get(1).unwrap_or(&5.to_string()).parse::<u32>().unwrap_or(5);
	let c2 = match args.get(2) { Some(d) => d.parse::<i32>().unwrap_or(5), None => 5 };
	println!("count = {}, {}", c1, c2);
}
