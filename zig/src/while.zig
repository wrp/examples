
const std = @import("std");

fn count() ?i32 {
	const state = struct {
		var counter: i32 = 0;
	};
	state.counter += 1;
	return if (state.counter == 3) null else state.counter;
}

pub fn main() !void {
	const stdout = std.io.getStdOut().writer();

	// Capture the value of an optional
	while (count()) |n| {
		try stdout.print("{d}\n", .{n});
	}

	// Demonstrate a continue expression
	var i: i32 = 0;
	while (i < 3) : (i += 1) {
		try stdout.print("i = {d}\n", .{i});
	}
}
