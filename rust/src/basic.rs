use std::io;

#[allow(dead_code)]

// #[derive(Copy, Clone)]
struct Nums { x: i32, y: usize, z: u64, }

// Instead of deriving, can implement the trait explicitly
impl Copy for Nums {}
impl Clone for Nums { fn clone(&self) -> Self { *self } }

fn main() -> io::Result<()> {
        let _v = vec![1, -2, 2, 3, -10, 3, 17, -1, 13, -1500, 5];  // Initialize a vec
	let _cur = Nums {x: 0, y: 0, z: 0};
	let mut _max = _cur;


	let mut v = vec![1,2,3];
	v.push(5);
	for i in &v { println!("{}", i); }
	for i in &mut v { *i += 5; println!("{}", i); }

	let s = String::from("Hello, World!");
	let w = &s[7..];
	println!("w = {}", w);
	Ok(())
}
