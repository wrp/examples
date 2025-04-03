//! Example using all the primitive types

pub fn main() void {
	// integers
	_ = @as(i8, 1);
	_ = @as(i8, 1);
	_ = @as(u8, 1);
	_ = @as(i16, 1);
	_ = @as(u16, 1);
	_ = @as(i32, 1);
	_ = @as(u32, 1);
	_ = @as(i64, 1);
	_ = @as(u64, 1);
	_ = @as(i128, 1);
	_ = @as(u128, 1);
	_ = @as(isize, 1); // pointer sized integer
	_ = @as(usize, 1); // unsigned pointer sized integer
	_ = @as(c_char, 1);
	_ = @as(c_short, 1);
	_ = @as(c_ushort, 1);
	_ = @as(c_int, 1);
	_ = @as(c_uint, 1);
	_ = @as(c_long, 1);
	_ = @as(c_ulong, 1);
	_ = @as(c_longlong, 1);
	_ = @as(c_ulonglong, 1);
	_ = @as(c_longdouble, 1);

	_ = @as(f16, 1);
	_ = @as(f32, 1);
	_ = @as(f64, 1);
	_ = @as(f80, 1);
	_ = @as(f128, 1);

	_ = @as(bool, true);

}
