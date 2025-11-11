//! Basic sample showing reading from a file.

const std = @import("std");

pub fn getchar(reader: *std.Io.Reader) !?u8 {
	// Read one char at a time, emulating libc's getchar
	return reader.takeByte() catch |err| switch (err) {
		error.EndOfStream => return null,
		else => return err,
	};
}


pub fn show_file(file: std.fs.File, io: std.Io, writer: *std.Io.Writer) !void {
	var file_buf: [4096]u8 = undefined;
	var file_reader = file.reader(io, &file_buf);
	const reader = &file_reader.interface;

	while (try getchar(reader)) |ch| {
		try writer.print("{c}", .{ch});
	}
}


pub fn main() !void {
	var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
	defer arena.deinit();

	const allocator = arena.allocator();

	const args = try std.process.argsAlloc(allocator);
	defer std.process.argsFree(allocator, args);

	// Initialize the I/O system
	var threaded = std.Io.Threaded.init(allocator);
	defer threaded.deinit();
	const io = threaded.ioBasic();

	var stdout_buf: [4096]u8 = undefined;
	var stdout_writer = std.fs.File.stdout().writer(&stdout_buf);
	const stdout = &stdout_writer.interface;

	var stderr_buf: [1024]u8 = undefined;
	var stderr_writer = std.fs.File.stderr().writer(&stderr_buf);
	const stderr = &stderr_writer.interface;

	for (args[1..]) |filename| {
		var path_buffer: [std.fs.max_path_bytes]u8 = undefined;
		const path = std.fs.realpath(filename, &path_buffer) catch |err| {
			try stderr.print("ERROR: cannot resolve path '{s}': {}\n", .{filename, err});
			try stderr.flush();
			continue;
		};

		const file = std.fs.openFileAbsolute(path, .{}) catch |err| {
			try stderr.print("ERROR: cannot open '{s}': {}\n", .{filename, err});
			try stderr.flush();
			continue;
		};
		defer file.close();

		if (args.len > 2){
			try stdout.print("{s}:\n", .{filename});
		}

		show_file(file, io, stdout) catch |err| {
			try stderr.print("ERROR reading '{s}': {}\n", .{filename, err});
			try stderr.flush();
			continue;
		};
	}

	try stdout.flush();
}
