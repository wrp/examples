
const std = @import("std");
const stdout = std.io.getStdOut().writer();

// Loop over arguments

pub fn main() !void {
	var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
	defer arena.deinit();
	const allocator = arena.allocator();

	const args = try std.process.argsAlloc(allocator);
	defer std.process.argsFree(allocator, args);

	for (args[1..]) |arg| {
		try stdout.print("{s}\n", .{arg});
	}
}
