// Compare strings, demonstrating comptime, etc.

const std = @import("std");

fn insensitive_eql(comptime uppr: []const u8, str: []const u8) bool {
	comptime {
		var i = 0;
		while (i < uppr.len) : (i += 1) {
			if (uppr[i] >= 'a' and uppr[i] <= 'z') {
				@compileError("`uppr` must be all uppercase");
			}
		}
	}
	var i: u32 = 0;
	while (i < uppr.len) : (i += 1) {
		const val =
			if (str[i] >= 'a' and str[i] <= 'z')
				str[i] - ('a' - 'A')
			else
				str[i];
		if (val != uppr[i])
			 return false;
	}
	return true;
}

pub fn main() !void {
	const x = insensitive_eql("HELLO", "hElLo");
	std.log.info("x = {}", .{x});
}

test "validate_comparison" {
	// Using FoO causes 'zig test' to fail at compile time
	// Since the error is in a test, 'zig run' works just fine
	try std.testing.expectEqual(true, insensitive_eql("FoO", "foo"));
}
