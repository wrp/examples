
use std::fmt::{self, Formatter, Display};

struct Point<T: Display> {
	x: T,
	y: T,
}

struct Point2<T1: Display, T2: Display> {
	x: T1,
	y: T2,
}

impl <T: Display> Display for Point <T> {
        // `f` is a buffer, and this method must write the formatted string into it
        fn fmt(&self, f: &mut Formatter) -> fmt::Result {
                // `write!` is like `format!`, but it will write the formatted string
                // into a buffer (the first argument)
                write!(f, "({0:.3},{1:.3})", self.x, self.y)
        }
}


fn main() {
	let i = Point { x: 5, y: 10 };
	let f = Point { x: 1.0, y: 4.0 };
	let z = Point2 { x: 1.0, y: 4 };

	println!("{}", i);
	println!("{}", f);
	println!("({},{})", z.x, z.y);
}
