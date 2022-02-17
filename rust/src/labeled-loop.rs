fn main() {
	let mut count = 0;
	let a = 'foo: loop {
		println!("count = {}", count);
		let mut remaining = 10;
		loop {
			println!("remaining = {}", remaining);
				if remaining == 9 {
					break 5;  // Branch to A
			}
			if count == 2 {
				break 'foo count;  // Branch to B, bind count to a
			}
			remaining -= 1;
		};
		// A
		count += 1;
	};
	// B
	println!("End count = {}, a = {a}", count);
}
