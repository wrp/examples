
const std = @import("std");

pub fn main() !void {
	var stdout = std.fs.File.stdout().writerStreaming(&.{});
	try stdout.interface.print("Hello, {s}!\n", .{"world"});
}
