
// Derive the `fmt::Debug` implementation for `Structure`. `Structure`
// is a structure which contains a single `i32`.
#[derive(Debug)]
struct Structure(i32);

// Put a `Structure` inside of the structure `Deep`. Make it printable
#[derive(Debug)]
struct Deep(Structure);

fn main() {
	// Printing with `{:?}` is similar to printing with `{}`.
	// The second 12 prints with double quotes.  (I speculate
	// that all strings arguments get printed with quotes.)

	println!("{:?} months in a year. {:?}", 12, "12");
	// {actor:?} prints double quotes and escapes the apostrophe.  '"actor\'s"'
	println!("{1:?} {0:?} is the {actor:?} actor's name.",
		"Slater",
		"Christian",
		actor="actor's"
	);

	// `Structure` is printable!
	println!("Now {:?} will print!", Structure(3));

	// The problem with `derive` is there is no control over how
	// the results look. What if I want this to just show a `7`?
	println!("Now {:?} will print!", Deep(Structure(7)));
}

