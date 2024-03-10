



// {[position][specifier]:[fill][alignment][width].[precision]}

//Position      The index of the argument that should be inserted
//Specifier     A type-dependent formatting option
//Fill          A single character used for padding
//Alignment     One of <^>; left, middle and right alignment
//Width         The total width of the field (characters)
//Precision     How many decimals a formatted number should have

const std = @import("std");

pub fn main() !void {
	const s = std.io.getStdOut().writer();
	const i:i8 = -6;
	const u:u8 = 6;
	try s.print("{{b}} misbehaves with signed int: -6 -> {b:0>8}\n", .{i});
	try s.print("To display signed, bitCast to unsigned: -6 ->", .{});
	try s.print("{b:0>8}\n", .{ @as(u8, @bitCast(i)) });
	try s.print("{{b}} behaves with unsigned: {b:0>8}\n", .{u});
}
