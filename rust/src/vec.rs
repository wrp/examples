

use std::io;

fn main() -> io::Result<()> {
	let mut v = vec![1,2,3];

	v.push(5);
	for i in &v {
		println!("{}", i);
	}

	for i in &mut v {
		*i += 5;
		println!("{}", i);
	}
	Ok(())
}
