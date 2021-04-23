// https://doc.rust-lang.org/stable/rust-by-example/flow_control/loop.html
// https://doc.rust-lang.org/stable/rust-by-example/flow_control/for.html

fn main() {
	let mut count = 0u32;

	println!("Let's count until infinity!");

	// Infinite loop
	loop {
		count += 1;

		if count == 3 {
			println!("three");

			// Skip the rest of this iteration
			continue;
		}

		println!("{}", count);

		if count == 5 {
			println!("OK, that's enough");

			// Exit this loop
			break;
		}
	}

	for i in 1..5 {
		println!("{}: looping 1,2,3,4", i);
	}
	for i in 1..=5 {
		println!("{}: looping 1,2,3,4,5", i);
	}
}

