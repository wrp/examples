intcode:
	-set -x; \
	zig build-exe intcode.zig || \
	rustc -C debug-assertions=no -C opt-level=3 intcode.rs || \
	gcc -o intcode intcode.c
