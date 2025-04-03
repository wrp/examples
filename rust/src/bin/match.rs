
// keyword: switch (switch -> match)

fn main() {
	let data = "hello, world!";

	for c in data.chars() {
		match c {
		'h' => print!("{}", 'h'),
		'l' | 'o' => print!("{}", 'x'),
		'v'..='z' => print!("{}", 'X'),
		_ => print!("{}", c)
		}
	}
}
