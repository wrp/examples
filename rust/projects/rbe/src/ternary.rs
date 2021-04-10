

/*
 * Some options for emulating the ternary operator.
 * For the life of me, I cannot fathom why anyone thinks
 * if cond { val1 } else { val2 } is cleaner than
 * cond ? val1 : val2, but see https://github.com/rust-lang/rust/issues/1698
 * There are some reasonable arguments there that could persuade me.
 */
fn main() {

	let mut r = 5;
	println!("sign of {}: {}", r, (r >= 0).then(|| 1).unwrap_or(-1));
	r = -6;
	println!("sign of {}: {}", r, (r >= 0).then(|| 1).unwrap_or(-1));

	println!("sign of {}: {}", r, if r >= 0 { 1 } else { -1 } );
}
