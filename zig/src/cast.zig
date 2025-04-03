

const std = @import("std");

pub fn main() !void {
	const stdout = std.io.getStdOut().writer();

	for(0..1) |i| { try stdout.print("for(0..1) type is {}\n", .{ @TypeOf(i) }); } // usize

	// Narrowing cast
	for(15..18) |us| {
		const u = @as(u4, @truncate(us));
		try stdout.print("@as(u4, @truncate({d}) -> {b:0>4}\t", .{us, u});

		// const i = @as(i4, @truncate(us));  comptime error! signedness
		const i = @as(i4,
				@truncate(
					@as(i64, @bitCast(us) // comptime error if usize not 64 bits
				)
			));
		try stdout.print("i4: {d}\n", .{i});
	}

	for([_]i8{-128, -127, -8, -7, -1, 1, 7, 127}) |i| {
		const narrow = @as(i4, @truncate(i));
		const u = @as(u8, @bitCast(i));
		try stdout.print("i8: {0}({1b:0>8} -> i4: {2}\n", .{ i, u, narrow });
	}

	for([_]i4{-8, -2, -1, 7}) |i| {
		const sm = @as(i3, @truncate(i));
		const u = @as(u4, @bitCast(i));
		try stdout.print("u: {d} d:{d} b:{b} sm:{b}\n", .{u, i, i, sm});
	}

	try stdout.print("EOL\n", .{});
}
