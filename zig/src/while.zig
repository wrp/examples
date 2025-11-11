
const std = @import("std");

fn count() ?i32 {
	const state = struct {
		var counter: i32 = 0;
	};
	state.counter += 1;
	return if (state.counter == 3) null else state.counter;
}

const CountError = error {
	Count3,
};
fn counter() !i32 {
	const state = struct { var counter: i32 = 0; };
	state.counter += 1;
	return if (state.counter == 3) CountError.Count3 else state.counter;
}

pub fn main() !void {
	var stdout_buf: [1024]u8 = undefined;
	var stdout_writer = std.fs.File.stdout().writer(&stdout_buf);
	const stdout = &stdout_writer.interface;

	var stderr_buf: [512]u8 = undefined;
	var stderr_writer = std.fs.File.stderr().writer(&stderr_buf);
	const stderr = &stderr_writer.interface;

	// Capture the value of an optional
	while (count()) |n| {
		try stdout.print("{d}\n", .{n});
	}

	// Capture the value of an error
	while (counter()) |n| {
		try stdout.print("{d}\n", .{n});
	} else |err| {
		try stderr.print("{}\n", .{err});
	}

	// Demonstrate a continue expression
	var i: i32 = 0;
	while (i < 3) : (i += 1) {
		try stdout.print("i = {d}\n", .{i});
	}

	try stdout.flush();
	try stderr.flush();
}
