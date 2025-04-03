
const std = @import("std");
const Repl = struct { div: usize, word: []const u8, };
pub fn convert(buffer: []u8, n: u32) []const u8 {
	var rv: []u8 = buffer[0..0];
	const replace = [_]Repl{
		.{ .div = 3, .word = "Pling" },
		.{ .div = 5, .word = "Plang" },
		.{ .div = 7, .word = "Plong" },
	};
	for (replace) |w| {
		if (n % w.div == 0) {
			var s = rv.len;
			rv.len += w.word.len;
			@memcpy(rv[s..], w.word);
		}
	}
	if (rv.len == 0) {
		rv = std.fmt.bufPrint(buffer, "{d}", .{n}) catch unreachable;
	}
	return rv;
}
