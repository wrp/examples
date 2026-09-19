
#include <cassert>
#include <iostream>

using namespace std;

// Example of a fold-expression

// The (...) syntax expands the operation across all passed arguments

template<typename... Args>
auto sum(Args... args) {
	return (args + ... ); // Right fold: return the sum of all arguments
}

int
main() {
	assert(sum(1, 2, 3, 4, 5) == 15);
	assert(sum(string("foo"), string("bar")) == string("foobar"));
}
