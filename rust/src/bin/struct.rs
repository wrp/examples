// Demonstrate derived std::fmt::Debug

#[derive(Debug)]
#[allow(dead_code)]
struct Rectangle {
	width: u32,
	height: u32,
	name: String,
}

#[derive(Debug)]
#[allow(dead_code)]
struct Point ( i32, i32, i32 ); // A tuple-struct

// Use "Field Init Shorthand
fn demo_field_init( height: u32, name: String) -> Rectangle {
	let width = height + 2;
	Rectangle { height, width, name }
}

impl Drop for Rectangle {
	fn drop(&mut self) {
		println!("Dropping Rectangle ({}, {}, {})", self.width, self.height, self.name);
	}
}


fn main() {
	let rect1 = Rectangle { width: 30, height: 50, name: "rect1".to_string() };
	let mut rect2 = demo_field_init(5, String::from("rect2"));

	let a = Point(2, 4, 5);
	println!("point is {:?}.  Middle is {}", a, a.1);

	// Use struct update.  Since all the copied fields implement the copy trait, this
	// does not borrow rect1
	let rect3 = Rectangle { name: String::from("Copied"), ..rect1 };

	println!("rect1 is {:?}", rect1);
	println!("rect2 is {:?}", rect2);
	println!("rect3 is {:?}", rect3);
	println!("height is {:#?}", rect1.height);

	println!("Assigning rect2 = rect1 to demonstrate drop is called for rect2");
	rect2 = rect1;	// Th
	println!("After assignment, rect2 is {:?}", rect2);
}
