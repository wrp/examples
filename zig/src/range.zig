
const std = @import("std");

pub fn main() !void {
	for (0..4) |x| {
		std.debug.print("{d}\n", .{x});
	}
}
