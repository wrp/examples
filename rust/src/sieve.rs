
/* Sieve of Eratosthenes */


use std::env;

fn main() {
	let args: Vec<String> = env::args().collect();
	let max = if args.len() > 1 { args[1].parse::<usize>().unwrap() } else {100usize};

	let mut x = vec![0; max];
	let mut c = 1;

	for p in 2..max {
		if x[p] != 0 {
			continue;
		}
		print!("{}{}", p, if c != 0 { '\t' } else { '\n'});
		c = (c + 1) % 8;
		for m in (2 * p..max).step_by(p) {
			x[m] = 1;
		}
	}
	if c != 1 {
		println!("");
	}
}
