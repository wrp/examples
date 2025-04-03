
const std = @import("std");
const stderr = std.io.getStdErr().writer();

pub fn read_stream(r: anytype) !u32 {
	var buf: [1024]u8 = undefined;

	var sum: u32 = 0;

	while (try r.readUntilDelimiterOrEof(&buf, '\n')) |line| {
		if (std.fmt.parseInt(u32, line, 10)) |value| {
			sum += value / 3 - 2;
		} else |err| {
			try stderr.print("Invalid: {}\n", .{err});
		}
	}
	return sum;
}

pub fn main() !void {
	var buffered_stdout = std.io.bufferedWriter(std.io.getStdOut().writer());
	const stdout = buffered_stdout.writer();
	defer buffered_stdout.flush() catch unreachable;

	const unbuffered_stdin = std.io.getStdIn().reader();
	var stdin = std.io.bufferedReader(unbuffered_stdin);
	const r = stdin.reader();

	if(read_stream(r)) |sum| {
		try stdout.print("{}\n", .{sum});
	} else |err| {
		try stderr.print("Error: {}\n", .{err});
	}
}
