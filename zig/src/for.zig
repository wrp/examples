

const std = @import("std");

pub fn main() !void {
	const stdout = std.io.getStdOut().writer();

	// Iterator type is usize
	for(0..3) |i| {
		try stdout.print("{d},", .{i});
	}

	// Narrowing cast
	for(0..3) |i| {
		const u = @as(u8, @truncate(i));
		try stdout.print("{d},", .{u});
	}

	const arr = [4]i8{1, 2, 3, 4};
	for(arr) |i| {
		try stdout.print("{d},", .{i});
	}
	try stdout.print("EOL\n", .{});

	for([_]i4{-8, -2, -1, 7}) |i| {
		const sm = @as(i3, @truncate(i));
		const u = @as(u4, @bitCast(i));
		try stdout.print("u: {d} d:{d} b:{b} sm:{b}\n", .{u, i, i, sm});
	}

	try stdout.print("EOL\n", .{});
}
