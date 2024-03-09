
const std = @import("std");

pub fn main() !void {

	// TODO: figure out how to cast away const so I can write:
	// const stdin_reader = std.io.bufferedReader(
	//         std.io.getStdIn().reader()
	// ).reader();
	// Currently (zig 0.11.0), the object returned by std.io.bufferedReader
	// is const, so the attempt to invoke reader() in the one-liner fails.
	var stdin = std.io.bufferedReader(std.io.getStdIn().reader());
	const stdin_reader = stdin.reader();

	var gpa = std.heap.GeneralPurposeAllocator(.{}){};
	defer _ = gpa.deinit();

	const alloc = gpa.allocator();

	var stack = std.ArrayList(u32).init(alloc);
	defer stack.deinit();

	if (read_stream(stdin_reader, &stack)) {
		try process(&stack);
	} else |err| {
		std.debug.print("error: {}\n", .{err});
	}

	var buffered_stdout = std.io.bufferedWriter(std.io.getStdOut().writer());
	const stdout = buffered_stdout.writer();
	defer buffered_stdout.flush() catch unreachable;
	try stdout.print("{d}\n", .{stack.items[0]});
}

pub fn read_stream(stream: anytype, data: *std.ArrayList(u32)) !void {
	var buf: [1024]u8 = undefined;

	// TODO: explore options for extant functions with multiple delims
	while (try stream.readUntilDelimiterOrEof(&buf, ',')) |text| {
		var len = text.len;
		if (len > 1 and text[len - 1] == '\n') {
			len -= 1;
		}
		try data.*.append(try std.fmt.parseInt(u32, text[0..len], 10));
	}
}

pub fn process(data: *std.ArrayList(u32)) !void {

	var i:u32 = 0;
	data.*.items[1] = 12;
	data.*.items[2] = 2;
	while (data.*.items[i] != 99) {
		const a = data.*.items[data.*.items[i + 1]];
		const b = data.*.items[data.*.items[i + 2]];
		const idx = data.*.items[i+3];

		switch (data.*.items[i]) {
		1 => data.*.items[idx] = a + b,
		2 => data.*.items[idx] = a * b,
		else => return error.FatalError,
		}
		i += 4;
	}
}
