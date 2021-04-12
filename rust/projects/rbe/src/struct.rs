// https://doc.rust-lang.org/stable/rust-by-example/custom_types/structs.html

#[derive(Debug)]
struct Person {
	name: String,
	age: u8,
}

// A unit struct
struct Unit;

// A tuple struct
struct Pair(i32, f32);

// A struct with two fields
#[derive(Debug)]
struct Point {
	x: f32,
	y: f32,
}

// Structs can be reused as fields of another struct
#[derive(Debug)]
struct Rectangle {
	top_left: Point,
	bottom_right: Point,
}

fn rect_area(r: Rectangle) -> f32 {
	(r.bottom_right.x - r.top_left.x).abs() * (r.top_left.y - r.bottom_right.y).abs()
}

fn square(p: Point, len: f32) -> Rectangle {
	Rectangle{
		    top_left: Point{ x: p.x, y: p.y + len },
		    bottom_right: Point{ x: p.x + len, y: p.y },
	}
}

fn main() {
	// Create struct with field init shorthand
	let name = String::from("Peter");
	let age = 27;
	let peter = Person { name, age };

	println!("{:?}", peter);

	let point: Point = Point { x: 10.3, y: 0.4 };
	println!("point coordinates: ({}, {})", point.x, point.y);

	// Make a new point wth struct update syntax
	let bottom_right = Point { x: 5.2, ..point };

	// `bottom_right.y`== `point.y`
	println!("second point: ({}, {})", bottom_right.x, bottom_right.y);

	// Destructure the point using a `let` binding
	let Point { x: top_edge, y: left_edge } = point;

	let r = Rectangle {
		// struct instantiation is an expression, too
		top_left: Point { x: left_edge, y: top_edge },
		bottom_right: bottom_right,
	};
	println!("rect: {:?}", r);
	println!("area: {}", rect_area(r));

	let r = square(Point { x: 1.0, y: 2.0}, 7.2);
	println!("area: {}", rect_area(r));

	// Instantiate a unit struct
	let _unit = Unit;

	// Instantiate a tuple struct
	let pair = Pair(1, 0.1);

	// Access the fields of a tuple struct
	println!("pair contains {:?} and {:?}", pair.0, pair.1);

	// Destructure a tuple struct
	let Pair(integer, decimal) = pair;

	println!("pair contains {:?} and {:?}", integer, decimal);
}
