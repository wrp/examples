

#include <iostream>
#include <vector>


// In C++, we can use an unnamed namespace to get internal linkage.

namespace {
	int foo(int);
};

int main()
{
	std::cout << foo(3) << '\n';
	return 0;
}

namespace {
	int foo(int x) { return x + 1; };
};
