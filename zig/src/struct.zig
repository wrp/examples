
const std = @import("std");
const math = std.math;
const assert = std.debug.assert;

// A struct definition doesn't include a name.
// Assigning the struct to a variable gives it a name.
const Point = struct {
	x: f64,
	y: f64,
	z: f64,

	// A struct definition can also contain namespaced functions.
	// This has no impact on the struct layout.
	// Struct functions that take a Self parameter, when
	// invoked through a struct instance, will automatically
	// fill the first argument, just like methods do.
	const Self = @This();
	pub fn distance(self: Self, p: Point) f64 {
		const x2 = math.pow(f64, self.x - p.x, 2);
		const y2 = math.pow(f64, self.y - p.y, 2);
		const z2 = math.pow(f64, self.z - p.z, 2);
		return math.sqrt(x2 + y2 + z2);
	}

	pub fn abs(s: Self) f64 {
		return math.sqrt(s.x * s.x + s.y * s.y + s.z * s.z);
	}
};

pub fn main() !void {
	const p1 = Point{ .x = 0, .y = 2, .z = 8 };
	const p2 = Point{ .x = 0, .y = 6, .z = 8 };
	assert(p1.distance(p2) == 4);
	assert(Point.distance(p1, p2) == 4);

	std.log.info("len = {}", .{p2.abs()});
}
