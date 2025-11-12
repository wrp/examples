//! RPN Calculator in Zig - simplified version of ../c/src/calc.c
//!
//! This is a Reverse Polish Notation calculator that supports:
//! - Basic arithmetic: +, -, *, /, ^
//! - Mathematical functions via \function syntax
//! - Stack operations: p (print), n (print/pop), y (duplicate), r (swap)
//! - Help: h for help, q to quit

const std = @import("std");
const math = std.math;

const help_text =
	\\RPN Calculator - Reverse Polish Notation
	\\
	\\Basic Operations:
	\\  +, -, *, /, ^  - Binary operators (pop 2, push result)
	\\  p              - Print top of stack
	\\  n              - Print and pop top of stack
	\\  y              - Duplicate top of stack
	\\  r              - Swap top two elements
	\\  C              - Clear (pop and discard)
	\\  Y              - Show entire stack
	\\  q              - Quit
	\\  h              - Show this help
	\\
	\\Functions (use \function syntax):
	\\  \sin, \cos, \tan, \sqrt, \exp, \log, \abs
	\\  \asin, \acos, \atan, \sinh, \cosh, \tanh
	\\
	\\Examples:
	\\  2 3 +          => 5
	\\  10 2 /         => 5
	\\  2 3 ^          => 8
	\\  45 \sin        => 0.8509...
	\\  9 \sqrt        => 3
	\\
;

const State = struct {
	stack: std.ArrayList(f64),
	allocator: std.mem.Allocator,

	fn init(allocator: std.mem.Allocator) State {
		return .{
			.stack = std.ArrayList(f64){},
			.allocator = allocator,
		};
	}

	fn deinit(self: *State) void {
		self.stack.deinit(self.allocator);
	}

	fn push(self: *State, value: f64) !void {
		try self.stack.append(self.allocator, value);
	}

	fn pop(self: *State) ?f64 {
		if (self.stack.items.len == 0) return null;
		return self.stack.pop();
	}

	fn peek(self: *State) ?f64 {
		if (self.stack.items.len == 0) return null;
		return self.stack.items[self.stack.items.len - 1];
	}
};

fn showValue(writer: *std.Io.Writer, value: f64) !void {
	try writer.print("{d}\n", .{value});
}

fn printStack(state: *State, writer: *std.Io.Writer) !void {
	for (state.stack.items, 0..) |val, i| {
		try writer.print("{d:3}: {d}\n", .{ i, val });
	}
}

fn executeFunction(state: *State, name: []const u8, stderr: *std.Io.Writer) !void {
	const arg1 = state.pop() orelse {
		try stderr.print("Stack empty for function: {s}\n", .{name});
		return;
	};

	const result = if (std.mem.eql(u8, name, "sin"))
		@sin(arg1)
	else if (std.mem.eql(u8, name, "cos"))
		@cos(arg1)
	else if (std.mem.eql(u8, name, "tan"))
		@tan(arg1)
	else if (std.mem.eql(u8, name, "asin"))
		std.math.asin(arg1)
	else if (std.mem.eql(u8, name, "acos"))
		std.math.acos(arg1)
	else if (std.mem.eql(u8, name, "atan"))
		std.math.atan(arg1)
	else if (std.mem.eql(u8, name, "sinh"))
		std.math.sinh(arg1)
	else if (std.mem.eql(u8, name, "cosh"))
		std.math.cosh(arg1)
	else if (std.mem.eql(u8, name, "tanh"))
		std.math.tanh(arg1)
	else if (std.mem.eql(u8, name, "sqrt"))
		@sqrt(arg1)
	else if (std.mem.eql(u8, name, "exp"))
		@exp(arg1)
	else if (std.mem.eql(u8, name, "log"))
		@log(arg1)
	else if (std.mem.eql(u8, name, "abs") or std.mem.eql(u8, name, "fabs"))
		@abs(arg1)
	else if (std.mem.eql(u8, name, "log10"))
		@log10(arg1)
	else if (std.mem.eql(u8, name, "log2"))
		@log2(arg1)
	else if (std.mem.eql(u8, name, "cbrt"))
		std.math.cbrt(arg1)
	else {
		try stderr.print("Unknown function: {s}\n", .{name});
		try state.push(arg1); // Push back
		return;
	};

	try state.push(result);
}

fn applyBinaryOp(state: *State, op: u8, stderr: *std.Io.Writer) !void {
	const b = state.pop() orelse {
		try stderr.print("Stack underflow for operator '{c}'\n", .{op});
		return;
	};
	const a = state.pop() orelse {
		try stderr.print("Stack underflow for operator '{c}'\n", .{op});
		try state.push(b); // Push back
		return;
	};

	const result: f64 = switch (op) {
		'+' => a + b,
		'-' => a - b,
		'*' => a * b,
		'/' => a / b,
		'^' => std.math.pow(f64, a, b),
		else => unreachable,
	};

	try state.push(result);
}

fn processToken(state: *State, token: []const u8, stdout: *std.Io.Writer, stderr: *std.Io.Writer) !void {
	if (token.len == 0) return;

	// Try to parse as number
	if (std.fmt.parseFloat(f64, token)) |value| {
		try state.push(value);
		return;
	} else |_| {}

	// Single character commands
	if (token.len == 1) {
		const c = token[0];
		switch (c) {
			'p' => {
				if (state.peek()) |val| {
					try showValue(stdout, val);
				} else {
					try stderr.print("Stack empty\n", .{});
				}
			},
			'n' => {
				if (state.pop()) |val| {
					try showValue(stdout, val);
				} else {
					try stderr.print("Stack empty\n", .{});
				}
			},
			'y' => {
				if (state.peek()) |val| {
					try state.push(val);
				} else {
					try stderr.print("Stack empty\n", .{});
				}
			},
			'r' => {
				const b = state.pop() orelse {
					try stderr.print("Stack underflow\n", .{});
					return;
				};
				const a = state.pop() orelse {
					try state.push(b);
					try stderr.print("Stack underflow\n", .{});
					return;
				};
				try state.push(b);
				try state.push(a);
			},
			'C' => {
				_ = state.pop();
			},
			'Y' => {
				try printStack(state, stdout);
			},
			'q' => {
				std.process.exit(0);
			},
			'h' => {
				try stdout.print("{s}", .{help_text});
			},
			'+', '-', '*', '/', '^' => {
				try applyBinaryOp(state, c, stderr);
			},
			else => {
				try stderr.print("Unknown command: {s}\n", .{token});
			},
		}
	} else {
		try stderr.print("Unknown command: {s}\n", .{token});
	}
}

pub fn main() !void {
	var gpa = std.heap.GeneralPurposeAllocator(.{}){};
	defer _ = gpa.deinit();
	const allocator = gpa.allocator();

	// Initialize I/O system
	var threaded = std.Io.Threaded.init(allocator);
	defer threaded.deinit();
	const io = threaded.ioBasic();

	var stdout_buf: [4096]u8 = undefined;
	var stdout_writer = std.fs.File.stdout().writer(&stdout_buf);
	const stdout = &stdout_writer.interface;

	var stderr_buf: [1024]u8 = undefined;
	var stderr_writer = std.fs.File.stderr().writer(&stderr_buf);
	const stderr = &stderr_writer.interface;

	var stdin_buf: [4096]u8 = undefined;
	var stdin_reader = std.fs.File.stdin().reader(io, &stdin_buf);
	const stdin = &stdin_reader.interface;

	var state = State.init(allocator);
	defer state.deinit();

	var input_buffer = std.ArrayList(u8){};
	defer input_buffer.deinit(allocator);

	// Read all input
	while (true) {
		const byte = stdin.takeByte() catch |err| switch (err) {
			error.EndOfStream => break,
			else => return err,
		};
		try input_buffer.append(allocator, byte);
	}

	// Process input
	var iter = std.mem.tokenizeAny(u8, input_buffer.items, " \t\n\r;");
	while (iter.next()) |token| {
		// Check for escape sequences
		if (token.len > 0 and token[0] == '\\') {
			const func_name = token[1..];
			try executeFunction(&state, func_name, stderr);
		} else {
			try processToken(&state, token, stdout, stderr);
		}
	}

	try stdout.flush();
	try stderr.flush();
}
