
use std::fmt::{self, Formatter, Display};

struct Point<T> {
	x: T,
	y: T,
}

struct Point2<T1, T2> {
	x: T1,
	y: T2,
}

impl Display for Point<i32> {
        // `f` is a buffer, and this method must write the formatted string into it
        fn fmt(&self, f: &mut Formatter) -> fmt::Result {
                // `write!` is like `format!`, but it will write the formatted string
                // into a buffer (the first argument)
                write!(f, "({},{})", self.x, self.y)
        }
}


fn main() {
	let i = Point { x: 5, y: 10 };
	let f = Point { x: 1.0, y: 4.0 };
	let z = Point2 { x: 1.0, y: 4 };

	println!("{}", i);
	println!("({0:.3},{1:.3})", f.x, f.y);
	println!("({},{})", z.x, z.y);
}
