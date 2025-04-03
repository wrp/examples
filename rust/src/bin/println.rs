

fn main() {

	let s = String::from("hello world");
	let x = 5;

	println!("s = '{}'", "hello");
	println!("s = '{1}, {0}!'", "world", "hello");      // numbered argument
	println!("s = '{0:>25}'", "hello");      // print with width 25
	println!("s = width from arg: '{0:>1$}'", "hello", 10);  // print with width 10
	println!("s = named arg: '{s}'", s = "hello");      // named argument
	println!("s = named arg with width: '{v:>50}'", v = "hello");
	println!("s = from enclosing scope: '{s}'");  // Take name from local scope (rustc >= 1.58)

	println!("x = {}", x);
	println!("var from enclosing scope x = {x}");
	println!("width of 4 with leading zeros x = {x:04}", x = 3);
}

