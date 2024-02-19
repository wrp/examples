
// Trivial demonstration sorting an array of i32

const std = @import("std");
const stdout = std.io.getStdOut().writer();

pub fn cmp(context: void, a: i32, b: i32) bool {
	return std.sort.asc(i32)(context, a, b);
}


pub fn main() !void {
	var a = [_]i32{5, 2, 1, -4, 7};
	try stdout.print("{s:15}: {any}\n", .{"unsorted", a});

	std.sort.heap(i32, a[0..], {}, cmp);
	try stdout.print("{s:15}: {any}\n", .{"ascending", a});

	std.sort.heap(i32, a[0..], {}, std.sort.desc(i32));
	try stdout.print("{s:15}: {any}\n", .{"descending", a});
}
