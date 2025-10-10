// Trivial example using an arrayList

const std = @import("std");

var gpa = std.heap.GeneralPurposeAllocator(.{}){};
var alloc = gpa.allocator();


pub fn build_array(data: anytype) !void {
	try data.*.append(alloc, 27);
	try data.*.append(alloc, 7);
	try data.*.append(alloc, 9);
}

pub fn main() !void {

	var stack: std.ArrayList(u32) = .{};
	defer stack.deinit(alloc);

	if (build_array(&stack)) {
		for (stack.items) |n| {
			std.debug.print("{},", .{n});
		}
		std.debug.print("\n", .{});
	} else |err| {
		std.debug.print("error: {}\n", .{err});
	}
}
