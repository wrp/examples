// Trivial demonstration copying data to a slice via memcpy
// With zig 95e9a6d6139a97406b36c1ed30dfac2dd (one commit from
// 96a5f7c8edac4bb2f50bdfe31c, which is just the application of:
//
// --- a/build.zig
// +++ b/build.zig
// @@ -623,7 +623,7 @@ fn addCompilerStep(b: *std.Build, options: AddCompilerStepOptions) *std.Build.St
//          .root_source_file = .{ .path = "src/main.zig" },
//          .target = options.target,
//          .optimize = options.optimize,
// -        .max_rss = 8_000_000_000,
// +        .max_rss = 3_000_000_000,
//          .strip = options.strip,
//          .sanitize_thread = options.sanitize_thread,
//          .single_threaded = options.single_threaded,
//
// The output is:

// copied 'abc len = 3'
// buffer 'abcXXXXXXXX len = 11'
// copied 'klmno len = 5'
// buffer 'abklmnoXXXX len = 11'

const std = @import("std");
var stdout_buffer: [1024]u8 = undefined;
var stdout_writer = std.fs.File.stdout().writer(&stdout_buffer);
const stdout = &stdout_writer.interface;


// copy first n bytes from src to dest
pub fn write_to_slice(dest: []u8, src: []const u8, comptime n: u32) []const u8 {
	// If n != src.len, this is a run-time panic
	@memcpy(dest[0..n], src);
	return dest[0..n];
}


pub fn main() !void {
	var buffer: [11]u8 = .{'X'} ** 11;
	const src = "abcdefghijklmnopqrstuv";

	{
	const d = write_to_slice(&buffer, src[0..3], 3);
	try stdout.print("copied '{s} len = {}'\n", .{d, d.len});
	try stdout.print("buffer '{s} len = {}'\n", .{buffer, buffer.len});
	}

	{
	const d = write_to_slice(buffer[2..], src[10..15], 5);
	try stdout.print("copied '{s} len = {}'\n", .{d, d.len});
	try stdout.print("buffer '{s} len = {}'\n", .{buffer, buffer.len});
	}
	try stdout.flush();
}
