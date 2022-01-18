
struct Point<T> {
	x: T,
	y: T,
}

struct Point2<T1, T2> {
	x: T1,
	y: T2,
}

fn main() {
	let i = Point { x: 5, y: 10 };
	let f = Point { x: 1.0, y: 4.0 };
	let z = Point2 { x: 1.0, y: 4 };

	println!("({},{})", i.x, i.y);
	println!("({0:.3},{1:.3})", f.x, f.y);
	println!("({},{})", z.x, z.y);
}
