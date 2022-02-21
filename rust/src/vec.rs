

use std::io;

fn main() -> io::Result<()> {
	let mut v = vec![7,9,3];

	v.push(5);
	for i in &v {
		println!("{}", i);
	}

	v.sort();
	println!("Sort and increment by 5");
	for i in &mut v {
		*i += 5;
		println!("{}", i);
	}
	Ok(())
}
