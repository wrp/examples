//! Read a stream of integers

const std = @import("std");
const stdout = std.io.getStdOut().writer();
const stderr = std.io.getStdErr().writer();

pub fn read_stream(r: anytype) !void {
	var buf: [8]u8 = undefined;

	while (try r.readUntilDelimiterOrEof(&buf, ' ')) |line| {
		if (std.fmt.parseInt(i32, line, 10)) |value| {
			try stdout.print("{d}\n", .{value});
		} else |err| {
			try stderr.print("Invalid: {}\n", .{err});
		}
	}
}

pub fn main() !void {

	var unbuffered_stdin = std.io.getStdIn().reader();
	var stdin = std.io.bufferedReader(unbuffered_stdin);
	var r = stdin.reader();

	if(read_stream(r)){

	} else |err| {
		try stderr.print("Error: {}\n", .{err});
	}
}
