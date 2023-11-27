//! Taken from https://www.reddit.com/r/Zig/comments/11intly/ive_been_trying_to_read_a_file_from_disk_into_a/
//! A first cut at reading a file.  (There are some bad practices here; just getting a
//! first cut at a working copy.)

const std = @import("std");
const stdout = std.io.getStdOut().writer();

const ProgramError = error{
	WrongAmountOfArguments,
};

pub fn main() !void {
	// Get an allocator.
	var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
	defer arena.deinit();
	const allocator = arena.allocator();

	// Get the file from the program's arguments, and assert there's a correct
	// amount of arguments.
	//
	// Note: you'd usually also want `std.process.argsFree` but since we're
	// using an arena allocator, the arena will free all memory at the end of
	// the program. Freeing at the end allows us to not worry about freeing
	// individual pieces of memory.
	// See also:
	//	"What's a Memory Allocator Anyway? - Benjamin Feng" on YouTube.
	const args = try std.process.argsAlloc(allocator);
	if (args.len != 2) {
		std.log.err(
			"Incorrect number of arguments: wanted 2, got {d}",
			.{args.len},
		);
		return ProgramError.WrongAmountOfArguments;
	}
	const filename = args[1];

	// Get the path to the file.
	//
	// We use `Z` version of `realpath` because Zig supports different types
	// of Pointer/Array notation. In this case, our arguments are 0-terminated
	// and that's the reason we use the `Z` variant.
	//
	// See also:
	//	"Solving Common Pointer Conundrums - Loris Cro" on YouTube.
	var path_buffer: [std.fs.MAX_PATH_BYTES]u8 = undefined;
	const path = try std.fs.realpathZ(filename, &path_buffer);

	// Open the file.
	//
	// The `.{}` means use the default version of `File.OpenFlags`.
	const file = try std.fs.openFileAbsolute(path, .{});
	defer file.close();

	// Get the contents - option 1
	// If the file is big prefer option 2.
	//
	// Read the entire file into memory by specifying a max length.
	// const mb = (1 << 10) << 10;
	// const file_contents = file.readToEndAlloc(allocator, 1 * mb);

	// Get the contents - option 2.
	//
	// Steam the file into memory using a buffered reader and get the contents
	// 1 byte at a time. You can change the number of bytes read at a time by
	// changing the buffer size.
	var buffered_file = std.io.bufferedReader(file.reader());
	var buffer: [1]u8 = undefined;
	while (true) {
		const number_of_read_bytes = try buffered_file.read(&buffer);

		if (number_of_read_bytes == 0) {
			break;
		}
		try stdout.print("{s}", .{buffer});
	}
}
