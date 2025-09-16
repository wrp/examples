const std = @import("std");

pub fn main() !void {
	var gpa = std.heap.GeneralPurposeAllocator(.{}){};
	defer _ = gpa.deinit();

	const allocator = gpa.allocator();
	const args = try std.process.argsAlloc(allocator);
	defer std.process.argsFree(allocator, args);

	if (args.len > 1) {
		const x = try std.fmt.parseInt(i32, args[1], 10);
		const y = x * x;
		std.debug.print("{d}\n", .{y});
	}

	// allocate a buffer of u8
	const b:[]u8 = try allocator.alloc(u8, 32);
	defer allocator.free(b);
	for (0.., b) |i, *v| {
		v.* = @as(u8, @intCast(i));
	}
	std.debug.print("{}: {d}\n", .{@TypeOf(b), b[31]});
}
