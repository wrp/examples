
const std = @import("std");
const stdout = std.io.getStdOut().writer();
const stderr = std.io.getStdErr().writer();

pub fn read_stream(r: anytype) !i32 {
	var buf: [1024]u8 = undefined;

	var sum: i32 = 0;

	while (try r.readUntilDelimiterOrEof(&buf, '\n')) |line| {
		if (std.fmt.parseInt(i32, line, 10)) |value| {
			var k: i32 = @divTrunc(value, 3) - 2;
			sum += k;
		} else |err| {
			try stderr.print("Invalid: {}\n", .{err});
		}
	}
	return sum;
}

pub fn main() !void {

	var unbuffered_stdin = std.io.getStdIn().reader();
	var stdin = std.io.bufferedReader(unbuffered_stdin);
	var r = stdin.reader();

	if(read_stream(r)) |sum| {
		try stdout.print("{}\n", .{sum});
	} else |err| {
		try stderr.print("Error: {}\n", .{err});
	}
}
