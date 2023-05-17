
use std::env;

fn main() {
	let args: Vec<String> = env::args().collect();
	let mut count = 0u32;

	// The first argument is the path that was used to call the program.
	println!("My path is {}.", args[0]);

	// The rest of the arguments are the passed command line parameters.
	// Call the program like this:
	//   $ ./args arg1 arg2
	println!("I got {:?} arguments: {:?}.", args.len() - 1, &args[1..]);

	// Even though we called .collect() on the iterator, we can still
	// iterate.  TODO: need to understand why we can do that
	for i in args.iter() {
		    println!("arg {:?}: {}", count, i);
		    count += 1;
	}
	// Indeed, we can iterate twice, and i here goes 3,4,5 (assuming 2 args)
	for i in args.iter() {
		    println!("arg {:?}: {}", count, i);
		    count += 1;
	}
}

