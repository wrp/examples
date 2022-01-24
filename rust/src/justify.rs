

use std::{
	env
};


/* Read text and justify on the given width */
fn main() {
	let args: Vec<String> = env::args().collect();
	let width = args[1].parse::<i32>().unwrap();

	println!("w = {}", width);


}
