
struct Point<T> {
	x: T,
	y: T,
}

fn main() {
	let i = Point { x: 5, y: 10 };
	let f = Point { x: 1.0, y: 4.0 };

	println!("integer = ({},{})", i.x, i.y);
	println!("float = ({0:.3},{1:.3})", f.x, f.y);
}
