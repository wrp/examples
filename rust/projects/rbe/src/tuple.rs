// https://doc.rust-lang.org/stable/rust-by-example/primitives/tuples.html

// Tuples can be used as function arguments and as return values
fn reverse(pair: (i32, bool)) -> (bool, i32) {
	// `let` can be used to bind the members of a tuple to variables
	let (integer, boolean) = pair;

	(boolean, integer)  // Implicit return
}

struct Matrix(f32, f32, f32, f32);
use std::fmt;
impl fmt::Display for Matrix {
	fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
		write!(f, "( {} {} )\n( {} {} )", self.0, self.1, self.2, self.3)
	}
}

fn main() {
	// A tuple with a bunch of different types
	let long_tuple = (
		1u8, 2u16, 3u32, 4u64,
		-1i8, -2i16, -3i32, -4i64,
		0.1f32, 0.2f64, 'a', true, false
	);

	// Values can be extracted from the tuple using tuple indexing
	println!("long tuple first value: {}", long_tuple.0);
	println!("long tuple second value: {}", long_tuple.1);

	// Tuples can be tuple members
	let tuple_of_tuples = ((1u8, 2u16, 2u32), (4u64, -1i8), -2i16);

	// Tuples are printable
	println!("tuple of tuples: {:?}", tuple_of_tuples);

	// But long Tuples cannot be printed
	// WTF!  This seems like a ridiculous feature of the language.  If the tuple
	// has less than 13 members, it seems to automatically implement std::fmt::Debug
	// but if it has 13 or more then the following line becomes a compile time error
	// println!("too long tuple: {:?}", long_tuple);

	let pair = (1, true);
	println!("pair is {:?}", pair);

	println!("the reversed pair is {:?}", reverse(pair));

	// To create one element tuples, the comma is required to tell them apart
	// from a literal surrounded by parentheses
	println!("one element tuple: {:?}", (5u32,));
	println!("just an integer: {:?}", (5u32));

	//tuples can be destructured to create bindings
	let tuple = (1, "hello", 4.5, true);

	let (a, b, c, d) = tuple;
	println!("{:?}, {:?}, {:?}, {:?}", a, b, c, d);

	let matrix = Matrix(1.1, 1.2, 2.1, 2.2);
	println!("{}", matrix);
}
