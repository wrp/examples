


fn main() {
	let mut s = String::from("hello");
	s.push_str(", world!");
	println!("{}", s);
	misc::public_function();
	misc::indirect_access();
}
