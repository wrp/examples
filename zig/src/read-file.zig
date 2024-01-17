//! Basic sample showing reading from a file.

const std = @import("std");
const stdout = std.io.getStdOut().writer();

pub fn main() !void {
	var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
	defer arena.deinit();

	const allocator = arena.allocator();

	const args = try std.process.argsAlloc(allocator);
	defer std.process.argsFree(allocator, args);

	for (args[1..]) |filename| {
		var path_buffer: [std.fs.MAX_PATH_BYTES]u8 = undefined;
		const path = try std.fs.realpathZ(filename, &path_buffer);

		const file = try std.fs.openFileAbsolute(path, .{});
		defer file.close();

		var buffered_file = std.io.bufferedReader(file.reader());
		var buffer: [1024]u8 = undefined;
		if (args.len > 2){
			try stdout.print("{s}:\n", .{filename});
		}
		while (true) {
			const n = try buffered_file.read(&buffer);

			if (n == 0) {
				break;
			}
			try stdout.print("{s}", .{buffer[0..n]});
		}
	}
}
