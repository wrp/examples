//https://doc.rust-lang.org/stable/rust-by-example/flow_control/loop/nested.html

#![allow(unreachable_code)]
#[allow(unused_labels)]

fn main() {
	let mut z = 1;
	let x = 'outer: loop {
		println!("Entered the outer loop. z = {}", z);

		let y = 'inner: loop {
			println!("Entered the inner loop. z = {}", z);

			// This break only the inner loop
			break z + 1;

			// This would break the outer loop
			break 'outer 5;
		};
		if y > 7 {
			break 'outer z;
		}
		z += 1;
	};

	println!("Exited the outer loop with avlue {}", x);
}

