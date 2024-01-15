

const std = @import("std");


pub fn main() !void {
	var node = LinkedList(i32).Node {
		.prev = null,
		.next = null,
		.data = 1234,
	};
	var node2 = LinkedList(i32).Node {
		.prev = null,
		.next = null,
		.data = 5678,
	};

	var list = LinkedList(i32) {
		.first = &node,
		.last = &node,
		.len = 1,
	};

	const stdout = std.io.getStdOut().writer();
	try stdout.print("{?}\n", .{list.first});
	try stdout.print("{?}\n", .{list.last});
	try stdout.print("{?}\n", .{usize});
	try stdout.print("{?}\n", .{list});

	try stdout.print("Value is: {d}\n", .{list.first.?.data});

	list.push(&node2);
	try stdout.print("new head is: {d}\n", .{list.first.?.data});
}

fn LinkedList(comptime T: type) type {
	return struct {
		pub const Node = struct {
			prev: ?*Node,
			next: ?*Node,
			data: T,
		};

		first: ?*Node,
		last:  ?*Node,
		len:   usize,

		pub fn push(self: *LinkedList(T), node: *Node) void {
			self.len += 1;
			node.next = self.first;
			self.first.?.prev = node;
			self.first = node;
		}
	};
}
