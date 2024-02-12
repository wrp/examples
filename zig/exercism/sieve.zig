
const std = @import("std");
const stdout = std.io.getStdOut().writer();

pub fn mark(b: []bool, v: usize) void {
	for (2..b.len / v) |c| {
		b[c * v] = false;
	}
}


pub fn primes(buffer: []u32, limit: u32) []u32 {
	var b:[2048]bool = undefined;
	b = .{true} ** b.len;  // Is it possible in zig to access .len in an initializer?

	if (limit > b.len) {
		@panic("Fixed size buffer is too small");
	}

	for (2..limit) |i| {
		if (b[i]) {
			mark(b[0..], i);
		}
	}
	var idx: u32 = 0;
	for (@as(u32, 2)..limit) |i| {  // i is usize: the @as cast does not coerce it to u32 ...
		if (b[i]) {
			buffer[idx] = @intCast(i);  // how to make i a u32 to avoid this cast?
			idx += 1;
		}
	}
	return buffer[0..idx];
}

pub fn main() !void {

	var b:[100]u32 = undefined; // .{5} ** 100;
	const v = primes(&b, 100);
	for (v) |c| {
		try stdout.print("{}\n", .{c} );
	}
}
