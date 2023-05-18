

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

	println!("{}: {:?}", "Mercury", solar_distance.get("Mercury").unwrap());
	for x in &args[1..] {
		match solar_distance.get(x.as_str()){
		Some(d) => println!("{}: {}", x, d),
		None => println!("{} not in map", x),
		}
	}

	// Use tuple as key
	let tuple_map = HashMap::from([((1,2), 5), ((2,3), 6)]);
	for (key, value) in &tuple_map {
		println!("{:?} -> {}", key, value);
	}

	let mut m = HashMap::new();
	m.insert("doug", 7);
	m.insert("alan", 4);
	for k in m.keys() {
		println!("{} -> {}", k, m[k]);
	}
}
