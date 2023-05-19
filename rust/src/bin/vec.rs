

use std::io;

fn main() -> io::Result<()> {
	let mut v = vec![7, 9, 3];

	v.push(5);
	println!("v = {:?}", v);
	v.pop();
	println!("after pop, v = {:?}", v);
	v.sort();
	println!("after sort, v = {:?}", v);
	for i in &mut v { *i += 5; }
	println!("after increment, v = {:?}", v);

	// Get default value from vec when out of range
	println!("Print {} elements (of {})", v.len() + 1, v.len());
	for i in 0..v.len() + 1 {
		let r = match v.get(i){ Some(d) => *d, None => -1 };
		print!("{r} ");
	}
	println!("");

	// Remove element from the vector
	let s = &v[1..];
	println!{"slice with first item removed: {:?}", s};

	Ok(())
}
