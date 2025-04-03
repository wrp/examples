const std = @import("std");
const sort = std.sort;

pub const Category = enum {
	ones,
	twos,
	threes,
	fours,
	fives,
	sixes,
	full_house,
	four_of_a_kind,
	little_straight,
	big_straight,
	choice,
	yacht,
};

const histogram = struct {
	count: [6]u3 = .{0} ** 6,
	sum: u5 = 0,
};

pub fn count(dice: [5]u3) !histogram {
	var a = histogram{};
	for (dice) |d| {
		if (d < 1 or d > 6) {
			return error.FatalError;
		}
		a.count[d - 1] += 1;
		a.sum += d;
	}
	return a;
}

pub fn exact(dice: [5]u3, value: u3) !u32 {
	return @as(u32, (try count(dice)).count[value - 1]) * value;
}

pub fn full_house(dice: [5]u3) !u32 {
	var m: u32 = 0;
	var sum: u32 = 0;
	for (1.., (try count(dice)).count) |v, c| {
		if (c == 3 or c == 2) {
			sum += c * @as(u32, @intCast(v));
			m += c;
		}
	}
	return if (m == 5) sum else 0;
}

pub fn four_of_a_kind(dice: [5]u3) !u32 {
	for (1.., (try count(dice)).count) |v, c| {
		if (c >= 4) {
			return @as(u32, @intCast(v)) * 4;
		}
	}
	return error.FatalError;
}

pub fn straight(dice: [5]u3, expect: [5]u3) !u32 {
	return if (std.mem.indexOfDiff(u3, dice[0..], expect[0..])) |_| 0 else 30;
}

pub fn yacht(dice: [5]u3) !u32 {
	const a = dice[0];
	for (dice) |d| {
		if (d != a) {
			return 0;
		}
	}
	return 50;
}

pub fn score(dice: [5]u3, category: Category) u32 {
	return score_e(dice, category) catch 0;
}

pub fn score_e(dice_u: [5]u3, category: Category) !u32 {
	var dice: [5]u3 = dice_u[0..5].*;
	sort.heap(u3, dice[0..], {}, sort.asc(u3));
	return
		switch(category) {
		Category.ones => exact(dice, 1),
		Category.twos => exact(dice, 2),
		Category.threes => exact(dice, 3),
		Category.fours => exact(dice, 4),
		Category.fives => exact(dice, 5),
		Category.sixes => exact(dice, 6),
		Category.full_house => full_house(dice),
		Category.four_of_a_kind => four_of_a_kind(dice),
		Category.little_straight => straight(dice, [5]u3{1,2,3,4,5}),
		Category.big_straight => straight(dice, [5]u3{2,3,4,5,6}),
		Category.choice => (try count(dice)).sum,
		Category.yacht => yacht(dice),
		};
}
