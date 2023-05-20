
use csv;
use std::io;
use rand::Rng;

fn main() -> io::Result<()> {
	let mut rdr = csv::Reader::from_reader(io::stdin());
	let mut rng = rand::thread_rng();

	for result in rdr.records() {
		let record = result.expect("a CSV record");
		println!("{:?}", record);
	}
	let v = rng.gen_range(0.0..10.0);
	println!("rand: {}", v);
	Ok(())
}
