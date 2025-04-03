
const std = @import("std");

pub const ColorBand = enum(usize) {
	black = 0,
	brown = 1,
	red = 2,
	orange = 3,
	yellow = 4,
	green = 5,
	blue = 6,
	violet = 7,
	grey = 8,
	white = 9,
};

pub fn colorCode(color: ColorBand) usize {
	return @intFromEnum(color);
}

pub fn colors() []const ColorBand {
	var v: [@typeInfo(ColorBand).Enum.fields.len]ColorBand = undefined;
	inline for (0.., std.meta.fields(ColorBand)) |i, f| {
		v[i] = @enumFromInt(f.value);
	}
	return &v;

	// return &[_]ColorBand{.black, .brown, .red, .orange,
	//	  .yellow, .green, .blue, .violet, .grey, .white};
}
