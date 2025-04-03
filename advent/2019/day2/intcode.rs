

use std::io::{self, BufRead};

fn main() -> io::Result<()> {
	let mut line = String::new();

	io::stdin().lock().read_line(&mut line).unwrap();

	// let v: Vec<&str> = line.split(",").collect();
	let mut v: Vec<usize> = line.trim().split(",").map(|x| x.parse::<usize>().unwrap()).collect();

	v[1] = 12;
	v[2] = 2;

	let mut i = 0;
	while v[i] != 99 {
		let a = v[i + 1];
		let b = v[i + 2];
		let c = v[i + 3];
		match v[i] {
		1 => v[c] = v[a] + v[b],
		2 => v[c] = v[a] * v[b],
		_ => panic!("Bad opcode at {}", i)
		}
		i += 4;
	}

	println!("{}", v[0]);
	Ok(())
}
