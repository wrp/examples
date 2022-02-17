fn main() {
	let mut count = 0;
	'foo: loop {
		println!("count = {}", count);
		let mut remaining = 10;
		loop {
			println!("remaining = {}", remaining);
				if remaining == 9 {
					break;  // Branch to A
			}
			if count == 2 {
				break 'foo;  // Branch to B
			}
			remaining -= 1;
		}
		// A
		count += 1;
	}
	// B
	println!("End count = {}", count);
}
