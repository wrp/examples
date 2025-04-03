// Quick sort integers

extern crate text_io;
use text_io::try_read;
use std::io;
use std::io::IsTerminal;

fn qsort(d: &mut [i32]) {
	if d.len() < 2 { return; }

	let len = d.len();
	let mut l: usize = 0;
	let mut r: usize = len - 2;
	let v = {
		if d[len - 1] < d[0] {
			(d[0], d[len - 1]) = (d[len - 1], d[0]);
		}
		d[len - 1]
	};
	while l < r {
		while d[l] < v { l += 1; }
		while d[r] > v { r -= 1; }
		if l < r {
			(d[l], d[r]) = (d[r], d[l]);
		}
	}
	if d[l] > v {
		(d[l], d[len - 1]) = (v, d[l]);
	}
	qsort(&mut d[0..l]);
	qsort(&mut d[r + 1..len]);
}

fn run_tests() {
	let test_cases = [
		vec![1,2,3],
		vec![3,2,1],
		vec![1],
		vec![1, 4],
		vec![4, 1],
		vec![1, 9, 2, 8, 3, 7, 4, 6, 5, 0],
	];
	let count = test_cases.len();
	for mut tc in test_cases {
		qsort(&mut tc);
		let mut s = tc[0];
		for v in &tc[1..] {
			if *v < s {
				eprintln!("Error sorting {:?}", tc);
			}
			s = *v;
		}
	};
	println!("Succesfully ran {} tests", count);
}


fn main() {
	let mut a: Result<i32, _>;
	let mut data = vec![];
	if io::stdin().is_terminal() {
		run_tests();
	} else {
		while {
			a = try_read!();
			a.is_ok()
		} {
			data.push(a.unwrap());
		}
		qsort(&mut data);
		println!("{:?}", data);
	}
}
