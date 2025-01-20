// Implementation of Euclid's algorithm

use std::{ io::{self} };


fn gcd(u1: u32, v1: u32) -> u32 {
	let (mut v, mut u) = (v1, u1);
	while u > 0 {
		if u < v {
			(v, u) = (u, v);
		}
		u = u - v;
	}
	return v;
}


fn main() -> io::Result<()> {
	for line in io::stdin().lines() {
		let [u, v] = line
			.unwrap()
			.trim()
			.split_whitespace()
			.map(|s| s.parse::<u32>().unwrap())
			.collect::<Vec<u32>>()[0..2] else { todo!() };
		println!("<{:?}, {:?}> -> {:?}", u, v, gcd(u, v));
	}
	Ok(())
}
