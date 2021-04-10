
fn main() {
	println!("1 + 2 = {}", 1u32 + 2);
	println!("1i32 - 2 = {}", 1i32 - 2);

	/* The follwing overlow Causes compileime error
	 * which can be suppressed with #[allow(const_err)],
	 * and then it becomes a runtime error.
	 */
	// println!("1u32 - 2 = {}", 1u32 - 2);

	// Short-circuiting boolean logic
	println!("true && false is {}", true && false);
	println!("true || false is {}", true || false);
	println!("NOT true is {}", !true);

	// Bitwise operations
	println!("0011 & 0101 is {:04b}", 0b0011u32 & 0b0101);
	println!("0011 | 0101 is {:04b}", 0b0011u32 | 0b0101);
	println!("0011 ^ 0101 is {:04b}", 0b0011u32 ^ 0b0101);
	println!("1u32 << 5 is {}", 1u32 << 5);
	println!("0x80 >> 2 is 0x{:x}", 0x80u32 >> 2);

	// Use underscores to improve readability!
	println!("One million can be written as 1_000_000u: {}", 1_000_000u32);
}

