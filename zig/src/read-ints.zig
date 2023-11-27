//! Read a stream of integers

const std = @import("std");

pub fn main() !void {
	const stdout = std.io.getStdOut().writer();

	var unbuffered_stdin = std.io.getStdIn().reader();
	var stdin = std.io.bufferedReader(unbuffered_stdin);
	var r = stdin.reader();
	var buf: [1000]u8 = undefined;
	while (try r.readUntilDelimiterOrEof(&buf, ' ')) |line| {
		const value = try std.fmt.parseInt(i32, line, 10);
		try stdout.print("{d}\n", .{value});
	}
}
