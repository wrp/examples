// Example from section 4.3 of the book

// Use string slice to find first word in a string
fn first_word(s: &String) -> &str {
	let bytes = s.as_bytes();
	let mut start : usize = 0;
	let mut seen : bool = false;

	for (i, &item) in bytes.iter().enumerate() {
		if item == b' ' && seen {
			return &s[start..i];
		} else if item != b' ' && ! seen {
			start = i;
			seen = true;
		}
	}

	&s[..]
}

fn main() {
	let s = String::from("     hello world");

	let word = first_word(&s);

	// s.clear(); // error!  (mutable borrow while word still has ref)

	println!("the first word is: '{}'", word);
}
