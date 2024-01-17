const std = @import("std");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();
    const args = try std.process.argsAlloc(allocator);
    defer std.process.argsFree(allocator, args);

    const x = try std.fmt.parseInt(i32, args[1], 10);
    const y = x * x;
    std.debug.print("{d}\n", .{y});
}
