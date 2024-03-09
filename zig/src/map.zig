

const std = @import("std");

pub fn string_map(stdout: anytype, allocator: anytype) !void {
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


pub fn main() !void
{
	const stdout = std.io.getStdOut().writer();
	var gpa = std.heap.GeneralPurposeAllocator(.{}){};
	defer _ = gpa.deinit();
	try string_map(stdout, gpa.allocator());
}
