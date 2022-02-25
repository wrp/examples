
#[derive(Debug)]
enum Foo {
	X(i32),
	Y(f32),
}

fn main() {
	let fx = Foo::X(2);
	let fy = Foo::Y(2.0);

	println!( "x = {:?}", fx);
	println!( "y = {:?}", fy);
}
