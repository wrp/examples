

const std = @import("std");

pub fn main() !void {
	const stdout = std.io.getStdOut().writer();
	var gpa = std.heap.GeneralPurposeAllocator(.{}){};
	defer _ = gpa.deinit();

	const allocator = gpa.allocator();
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

	//for (m.keys()[0..], m.values()[0..], 0..) |*key, *value, i| {
	//	const k = key.*;
	//	const v = value.*;
	//	try stdout.print("{d}, {d}: {d}!\n", .{i, k, v});
	//}
}
