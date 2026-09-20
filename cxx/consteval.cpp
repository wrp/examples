

#include <cassert>

constexpr int run(int i) {
	if consteval {
		return i*2;
	} else {
		return i;
	}
}

int
main() {
	static_assert(run(10) == 20); // compile-time
	assert(run(10) == 10);        // run time
}
