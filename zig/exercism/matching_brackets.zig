const std = @import("std");
const mem = std.mem;

/// Return true if the characters (, [, and { are correctly paired in s.
pub fn isBalanced(alloc: mem.Allocator, s: []const u8) !bool {
	var stack = std.ArrayList(u8).init(alloc);
	defer stack.deinit();

	for (s) |c| {
		switch (c) {
		'(' => { try stack.append(')'); },
		'[' => { try stack.append(']'); },
		'{' => { try stack.append('}'); },
		')', '}', ']' => {
			if (stack.popOrNull()) |k| {
				if (k != c) {
					return false;
				}
			} else {
				return false;
			}
		},
		else => continue,
		}
	}
	if (stack.getLastOrNull()) |_| {
		return false;
	}
	return true;
}
