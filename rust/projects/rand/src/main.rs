

/* problem description:
   generate an order book consisting of two lists of tuples (p, q),
   one ascending, one descending.  Update the lists by chosing a value
   q and then popping enough items off one list to sum up to q and sum
   the p values used.
*/

// extern crate rand;
use std::{ env, io };
use rand::Rng;

#[derive(PartialEq, PartialOrd, Debug)]
struct Order {
	p: f64,
	q: f64
}


struct Book {
	sell: Vec<Order>,
	buy: Vec<Order>
}

impl Book {
	fn sell(&mut self, s: &Order) {
		println!("selling: {:?}", s);
		let mut v = s.q;
		while v > 0.0 {
			let b = &mut self.buy[0];
			if b.q > v {
				b.q -= v;
				v = 0.0;
			} else {
				v -= b.q;
				// TODO: don't use remove.  Store a slice in the struct
				self.buy.remove(0);
			}
		}
	}
	fn print(&self) {
		println!("sells:");
		for s in &self.sell { println!("{:?}", s); }
		println!("buys:");
		for s in &self.buy { println!("{:?}", s); }
	}
}

fn main() -> io::Result<()> {
	let arg: Vec<String> = env::args().collect();
	let o_count = match arg.get(1) { Some(d) => d.parse::<i32>().unwrap_or(5), None => 5 };
	let s_count = match arg.get(2) { Some(d) => d.parse::<i32>().unwrap_or(5), None => 5 };
	let mut b = Book{sell: Vec::new(), buy: Vec::new()};
	let mut rng = rand::thread_rng();

	for _ in 0..o_count {
		b.buy.push(Order{p: rng.gen_range(0.0..10.0), q: rng.gen_range(1.0..20.0)})
	}
	for _ in 0..s_count {
		b.sell.push(Order{p: rng.gen_range(0.0..10.0), q: rng.gen_range(1.0..20.0)})
	}
	b.buy.sort_by(|a, b| a.partial_cmp(b).unwrap());
	b.sell.sort_by(|b, a| a.partial_cmp(b).unwrap());
	b.print();
	b.sell(&Order{p:2.3, q:1.0});
	b.print();


	Ok(())
}
