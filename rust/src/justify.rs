

use std::{
	env
};


/* Read text and justify on the given width */
fn main() {
	let args: Vec<String> = env::args().collect();
//	let word = &*args.get(4).unwrap_or(&"80".to_string());

//	let width = match &args.get(1) {
//		Some(&s) => s.parse::<i32>().unwrap(),
//		None => 80,
//	};
	let width = args.get(1).unwrap_or(&"80".to_string()).parse::<i32>().unwrap();

	println!("w = {}", width);


}
