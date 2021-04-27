// https://doc.rust-lang.org/stable/rust-by-example/flow_control/if_let.html

fn main() {
	// All have type `Option<i32>`
	let number = Some(7);
	let letter: Option<i32> = None;
	let emoticon: Option<i32> = None;

	// The `if let` construct reads: "if `let` destructures `number` into
	// `Some(i)`, evaluate the block (`{}`).
	if let Some(i) = number {
		println!("Matched {:?}!", i);
	}

	// If you need to specify a failure, use an else:
	if let Some(i) = letter {
		println!("Matched {:?}!", i);
	} else {
		// Destructure failed. Change to the failure case.
		eprintln!("Didn't match a number. Let's go with a letter!");
	}

	// Provide an altered failing condition.
	let i_like_letters = false;

	if let Some(i) = emoticon {
		println!("Matched {:?}!", i);
	// Destructure failed. Evaluate an `else if` condition to see if the
	// alternate failure branch should be taken:
	} else if i_like_letters {
		eprintln!("Didn't match a number. Let's go with a letter!");
	} else {
		// The condition evaluated false. This branch is the default:
		eprintln!("I don't like letters. Let's go with an emoticon :)!");
	}
	main2()
}

#[derive(Debug)]
enum Foo {
	Bar,
	Baz,
	Qux(u32)
}

fn main2() {
	// Create example variables
	let a = Foo::Bar;
	let b = Foo::Baz;
	let c = Foo::Qux(100);

	// Variable a matches Foo::Bar
	if let Foo::Bar = a {
		println!("a is foobar");
	}

	// Variable b does not match Foo::Bar
	// So this will print nothing
	if let Foo::Bar = b {
		println!("b is foobar");
	}

	// Variable c matches Foo::Qux which has a value
	// Similar to Some() in the previous example
	if let Foo::Qux(value) = c {
		println!("c is {:?} value is {}", c, value);
	}

	// Binding also works with `if let`
	if let Foo::Qux(value2 @ 100) = c {
		println!("value2 is {}", value2);
	}
}
