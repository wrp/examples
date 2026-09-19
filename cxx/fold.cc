
#include <cassert>
#include <iostream>

using namespace std;

// Example of a fold-expression

// The (...) syntax expands the operation across all passed arguments

template<typename... Args>
auto sum(Args... args) {
	return (args + ... ); // Right fold: return the sum of all arguments
}

template<typename... Args>
auto right_subtract(Args... args) {
	return (args - ...); // Right fold: (a - (b - c))
}

template<typename... Args>
auto left_subtract(Args... args) {
	return (... - args); // Left fold: ((a - b) - c)
}


int
main() {
	assert(sum(1, 2, 3, 4, 5) == 15);
	assert(sum(string("foo"), string("bar")) == string("foobar"));

	assert(right_subtract(5, 4, 3) == 4);
	assert(left_subtract(5, 4, 3) == -2);
}
