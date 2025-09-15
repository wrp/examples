// Simple demonstrations of basic control structures

const std = @import("std");

fn for_loops(stdout: *std.Io.Writer) !void {
	const names = [_][]const u8{"Alice", "Bob"};  // Initialize array of strings
	const text = "banana";

	for (text) |c| {
		try stdout.print("{c}", .{c});
	}
	try stdout.print("\n", .{});

	// Basic usage
	for(0..3) |i| {
		const delim = if(i==2) "\n" else ", ";
		try stdout.print("{d}{s}", .{i, delim});
	}

	// Iterate over an array
	for(names) |n| {
		try stdout.print("{s}\n", .{n});
	}

	// Enumerate an array
	for(0.., names) |i, n| {
		const delim = if(i==1) "\n" else ", ";
		try stdout.print("{s}{s}", .{n, delim});
	}

	// Else clause is only executed on non-breaking exit
	var a: usize = undefined;
	for(0..3) |i| {
		a = i;
	} else {
		// In the else clause, captured i is not available
		try stdout.print("in else: a = {d}\n", .{a});
	}
}

const ParityError = error {
	IsOdd,
	IsEven,
};

fn is_odd(x: i32) !bool {
	if (@rem(x, 2) == 0) {
		return ParityError.IsEven;
	}
	return true;
}

fn conditionals(stdout: *std.Io.Writer) !void {
	// capture clause on the if captures the non-error value
	if (is_odd(8)) |a| {
		try stdout.print("Cannot happen, that {any} is true\n", .{a});
	// capture on the else captures the error
	} else |e| {
		try stdout.print("8 is not odd: {}\n", .{e});
	}
}


pub fn main() !void {
	var stdout_buffer: [1024]u8 = undefined;
	var stdout_writer = std.fs.File.stdout().writer(&stdout_buffer);
	const stdout = &stdout_writer.interface;

	try for_loops(stdout);
	try conditionals(stdout);
	try stdout.flush();
}
