

use std::collections::HashMap;
use std::env;

fn main() {
	let args: Vec<String> = env::args().collect();

	let solar_distance = HashMap::from([
		("Mercury", 0.4),
		("Venus", 0.7),
		("Earth", 1.0),
		("Mars", 1.5),
	]);

	println!(":{}: {:?}", "Mercury", solar_distance.get("Mercury").unwrap());
	for x in &args[1..] {
		match solar_distance.get(x.as_str()){
		Some(d) => println!("{}: {}", x, d),
		None => println!("{} not in map", x),
		}
	}
}
