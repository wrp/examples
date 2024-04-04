
const std = @import("std");

pub fn convert(buffer: []u8, n: u32) []const u8 {
	var base: usize = 0;
	if (n % 3 == 0) {
		@memcpy(buffer[base..base + 5], "Pling");
		base += 5;
	}
	if (n % 5 == 0) {
		@memcpy(buffer[base..base + 5], "Plang");
		base += 5;
	}
	if (n % 7 == 0) {
		@memcpy(buffer[base..base + 5], "Plong");
		base += 5;
	}
	if (base == 0) {
	    const s = std.fmt.bufPrint(buffer, "{d}", .{n}) catch "1";
	    base = s.len;
	}
	return buffer[0..base];
}
