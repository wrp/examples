

use std::{
	env
};


/* Read text and justify on the given width */
fn main() {
	let args: Vec<String> = env::args().collect();
	let width = match args.get(1) {
		Some(&s) => s.parse::<i32>().unwrap(),
		None => 80,
	};

	println!("w = {}", width);


}
