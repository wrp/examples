
const std = @import("std");

pub const ChessboardError = error {
	IndexOutOfBounds,
};

pub fn square(index: usize) ChessboardError!u64 {
	if (index > 64 or index == 0) {
		return ChessboardError.IndexOutOfBounds;
	}
	const v: u64 = std.math.pow(usize, 2, index - 1);
	return v;
}

pub fn total() u64 {
	var sum: u128 = 0;
	//for(0..64) |i| {
	//	  sum += square(i + 1) catch unreachable;
	//}
	sum = @as(u128, 2) * (square(64) catch unreachable) - 1;
	return @truncate(sum);
}
