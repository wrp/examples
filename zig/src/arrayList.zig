// Trivial example using an arrayList

const std = @import("std");

pub fn build_array(data: anytype) !void {
	try data.*.append(27);
	try data.*.append(7);
	try data.*.append(9);
}

pub fn main() !void {
	var gpa = std.heap.GeneralPurposeAllocator(.{}){};
	defer _ = gpa.deinit();

	const alloc = gpa.allocator();

	var stack = std.ArrayList(u32).init(alloc);
	defer stack.deinit();

	if (build_array(&stack)) {
		for (stack.items) |n| {
			std.debug.print("{},", .{n});
		}
		std.debug.print("\n", .{});
	} else |err| {
		std.debug.print("error: {}\n", .{err});
	}
}
