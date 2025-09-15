

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

	var stdout_buffer: [1024]u8 = undefined;
	var stdout_writer = std.fs.File.stdout().writer(&stdout_buffer);
	const stdout = &stdout_writer.interface;

	try stdout.print("{any}\n", .{list.first});
	try stdout.print("{any}\n", .{list.last});
	try stdout.print("{any}\n", .{usize});
	try stdout.print("{any}\n", .{list});

	try stdout.print("Value is: {d}\n", .{list.first.?.data});

	list.push(&node2);
	try stdout.print("new head is: {d}\n", .{list.first.?.data});
	try stdout.flush();
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
