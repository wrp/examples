

const std = @import("std");

pub fn
string_map(stdout: anytype, allocator: anytype)
!void
{
	const map = std.StringHashMap(usize);

	var m = map.init(allocator);
	defer m.deinit();

	const names = [_][]const u8{"one", "two", "three"};
	for(names, 1..4) |n, i| {
		_ = try m.fetchPut(n, i);
	}

	var it = m.iterator();
	while (it.next()) |kv| {
		try stdout.print("{s}: {d}\n", .{kv.key_ptr.*, kv.value_ptr.*});
	}
}

pub fn
int_map(stdout: anytype, allocator: anytype)
!void
{
	const map = std.AutoHashMap(u8, u8);

	var m = map.init(allocator);
	defer m.deinit();

	for(0..4) |iu| {
		const i = @as(u8, @truncate(iu));
		_ = try m.fetchPut(i, i * i);
	}

	var it = m.iterator();
	while (it.next()) |kv| {
		try stdout.print("{d}: {d}\n", .{kv.key_ptr.*, kv.value_ptr.*});
	}
}


pub fn main() !void
{
	const stdout = std.io.getStdOut().writer();
	var gpa = std.heap.GeneralPurposeAllocator(.{}){};
	defer _ = gpa.deinit();
	try string_map(stdout, gpa.allocator());
	try int_map(stdout, gpa.allocator());
}
