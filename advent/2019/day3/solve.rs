/*
sample input
    R75,D30,R83,U83,L12,D49,R71,U7,L72
    U62,R66,U55,R34,D71,R55,D58,R83 = distance 159
    R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
    U98,R91,D20,R16,D67,R40,U7,R15,U6,R7 = distance 135
*/

#![allow(non_camel_case_types)]
use std::str::FromStr;
use std::io::{self, BufRead};
use std::collections::HashMap;

#[derive(Debug)]
struct motion {
	direction: char,
	distance: u32
}


#[derive(Debug)]
struct parse_motion_error;

impl FromStr for motion {
	type Err = parse_motion_error;
	fn from_str(s: &str) -> Result<Self, Self::Err> {
		let dir = s.chars().nth(0).unwrap();
		let dist = s[1..].parse::<u32>().unwrap();
		Ok(motion { direction: dir, distance: dist })
	}
}

struct path {
	p: HashMap<(i32,i32), i32>,
	x: i32,
	y: i32,
	min: i32
}

impl path {
	fn push(&mut self, m: &motion, i: i32){
		let mut d = m.distance;
		while d > 0 {
			match m.direction {
			'U' => self.y -= 1,
			'D' => self.y += 1,
			'L' => self.x -= 1,
			'R' => self.x += 1,
			_ => panic!("bad input")
			}

			match self.p.get(&(self.x, self.y)) {
			Some(old) => if *old != i && (self.min == 0 ||
				self.x.abs() + self.y.abs() < self.min)
			{
				self.min = self.x.abs() + self.y.abs()
			},
			None => ()
			}
			self.p.insert((self.x, self.y), i);
			d -= 1;
		}
	}
}


fn main() -> io::Result<()> {
	let mut line = String::new();
	let mut p1 = path{ p: HashMap::new(), x:0, y:0, min:0 };

	io::stdin().lock().read_line(&mut line).unwrap();
	for v in line.trim().split(",").map(|x| x.parse::<motion>().unwrap()) {
		p1.push(&v, 1);
	}

	p1.x = 0; p1.y = 0;
	line.clear();
	io::stdin().lock().read_line(&mut line).unwrap();
	for v in line.trim().split(",").map(|x| x.parse::<motion>().unwrap()) {
		p1.push(&v, 2);
	}
	println!("min = {}", p1.min);

	Ok(())
}
