//https://doc.rust-lang.org/stable/rust-by-example/flow_control/loop/nested.html

#![allow(unreachable_code)]
#[allow(unused_labels)]

fn main() {
	let x = 'outer: loop {
		println!("Entered the outer loop");

		'inner: loop {
			println!("Entered the inner loop");

			// This would break only the inner loop
			//break;

			// This breaks the outer loop
			break 'outer 5;
		}

		println!("This point will never be reached");
	};

	println!("Exited the outer loop with avlue {}", x);
}

