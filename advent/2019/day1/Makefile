.PHONY: all
all: solve c-solve


solve:
	rustc solve.rs


c-solve:
	$(CC) solve.c -o $@

zig-solve: solve.zig
	zig build-exe --name $@ $<
