//! Read a stream of integers

const std = @import("std");

pub fn read_stream(r: *std.Io.Reader, stdout: *std.Io.Writer, stderr: *std.Io.Writer) !void {
	while (true) {
		const line = (r.takeDelimiter(' ') catch |err| switch (err) {
			error.ReadFailed => return err,
			error.StreamTooLong => {
				try stderr.print("Invalid: line too long\n", .{});
				try stderr.flush();
				continue;
			},
		}) orelse break;

		const trimmed = std.mem.trim(u8, line, &std.ascii.whitespace);
		if (trimmed.len == 0) continue;

		if (std.fmt.parseInt(i32, trimmed, 10)) |value| {
			try stdout.print("{d}\n", .{value});
		} else |err| {
			try stderr.print("Invalid: {}\n", .{err});
			try stderr.flush();
		}
	}
}

pub fn main() !void {
	var gpa = std.heap.GeneralPurposeAllocator(.{}){};
	defer _ = gpa.deinit();
	const allocator = gpa.allocator();

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

	var stdin_buf: [4096]u8 = undefined;
	var stdin_reader = std.fs.File.stdin().reader(io, &stdin_buf);
	const stdin = &stdin_reader.interface;

	if (read_stream(stdin, stdout, stderr)) {
		try stdout.flush();
	} else |err| {
		try stderr.print("Error: {}\n", .{err});
		try stderr.flush();
	}
}
