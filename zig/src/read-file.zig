//! Basic sample showing reading from a file.

// See OneByteReadReader struct in the OneByte test in std/io/buffered_tee.zig

const std = @import("std");
const stdout = std.io.getStdOut().writer();
const stderr = std.io.getStdErr().writer();

pub fn getchar(file: anytype) !?u8 {
	// Read one char at a time, emulating libc's getchar

	var buffer: [1]u8 = undefined;
	const n = try file.*.read(&buffer);
	if (n == 0) {
		return null;
	} else {
		return buffer[0];
	}
}


pub fn show_file(file: std.fs.File) !void {
	var buffered_file = std.io.bufferedReader(file.reader());
	var buf_stdout = std.io.bufferedWriter(stdout);
	var buf_out = buf_stdout.writer();

	while (getchar(&buffered_file)) |n| {
		if (n == null) {
			try buf_stdout.flush();
			return;
		}
		try buf_out.print("{c}", .{n.?});
	} else |err| {
		try stderr.print("ERROR: {}\n", .{err});
	}
}


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

		if (args.len > 2){
			try stdout.print("{s}:\n", .{filename});
		}
		try show_file(file);
	}
}
