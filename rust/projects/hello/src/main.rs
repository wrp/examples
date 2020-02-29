fn main() {
	let mut s = String::from("Hello");
	s.push_str("world!");
	print_msg(&s);  // Pass reference to prevent move
}



fn print_msg(s: &String) {
	let hello = &s[0..5];
	let world = &s[5..];
	println!("{}, {}", hello, world);
}
