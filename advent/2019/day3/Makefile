solve: solve.rs solve.zig distance.c
	set -x; \
	zig build-exe solve.zig || \
	rustc -C debug-assertions=no -C opt-level=3 solve.rs || \
	gcc -o solve distance.c
