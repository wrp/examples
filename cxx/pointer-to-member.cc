
/* Demonstrate taking a reference to various member functions */

#include <iostream>
#include <vector>

using namespace std;

struct foo {
	int x;
	int y;
	int incr(int a) const { return a + x; };
	int decr(int a) const { return a - x; };
	unsigned uincr(int a) const { return a + x; };
	unsigned udecr(int a) const { return a - x; };
	foo(int a, int b) : x{a}, y{b} {};
};

template <typename T>
using memberFunc = T (foo::*)(int) const;
template <typename T>
struct exp {
	memberFunc<T> getter;
	int expected_value;
	std::string name;
};


int main()
{
	std::vector<exp<int>> v{
		{ &foo::incr, 3, "incr" },
		{ &foo::decr, 1, "decr" },
//		{ memberFunc<unsigned>&foo::uincr, 2, "uincr" },
	};
	foo b(1, 2);
	for(auto f: v) {
		int actual = ((b).*(f.getter))(2);

		cout << actual << '\n';
	};
	return 0;
}
