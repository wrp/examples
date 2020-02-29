
#![allow(non_camel_case_types)]
struct greeting {
	salutation: String,
	name: String,
}

fn main() {
	let mut s = String::from("Hello");
	s.push_str("world!");
	print_msg(&s);  // Pass reference to prevent move
}



fn print_msg(s: &str) {
	let g = greeting {
		salutation: s[0..5].to_string(),
		name: s[5..].to_string(),
	};
	println!("{}, {}", g.salutation, g.name);
}
