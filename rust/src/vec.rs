

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

	println!("Print 5 elements (of 4)");
	# Get default value from vec when out of range
	for i in 0..5 {
		let r = match v.get(i){ Some(d) => *d, None => -1 };
		println!("{:?}", r);
	}

	Ok(())
}
