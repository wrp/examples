fn main() {
	let mut s = String::from("hello");
	s.push_str(", world!");
	print_msg(&s);  // Pass reference to prevent move
	println!("{}", s);  // Still valid
}



fn print_msg(s: &String) {
	println!("{}", s);
}
