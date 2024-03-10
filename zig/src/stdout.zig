//! Write to buffered stdout

const std = @import("std");

pub fn main() !void {
	var buffered_writer = std.io.bufferedWriter(std.io.getStdOut().writer());
	defer buffered_writer.flush() catch unreachable;
	var stdout = buffered_writer.writer();

	const s = "abcdefg";
	for (s) |n| {
		try stdout.print("{c}", .{n});
	}
	stdout.print("\n", .{}) catch unreachable;
}
