


const std = @import("std");


pub fn main() !void {
	const x = comptime 3;
	const y = comptime squareComptime(x);
	std.debug.print("{d}\n", .{y});
}


pub fn squareComptime(comptime x: comptime_int) comptime_int {
	return x * x;
}

pub inline fn minComptime(comptime a: comptime_int, comptime b: comptime_int) comptime_int {
	return if (a < b) a else b;
}

test "squareComptime" {
	try std.testing.expectEqual(9, squareComptime(3));
	try std.testing.expectEqual(25, squareComptime(3 + 2));
}

test "minComptime" {
	try std.testing.expectEqual(minComptime(0, 0), 0);
	try std.testing.expectEqual(minComptime(30000000, 30000000000), 30000000);
	try std.testing.expectEqual(minComptime(-10, -3), -10);
}
