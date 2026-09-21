

#include <iostream>
#include <vector>

class foo {
public:
	double x{1.0};
	double y{2.0};
	std::string name{"unknown"};

	foo() {};
	foo(double x, double y) : x{x}, y{y} {}
	foo(double x, double y, std::string name) : x{x}, y{y}, name{name} {}
	foo(std::string name) : name{name} {};

	void incr(double a) { x += a; y -= a; }
	void bar(int);

	void print(std::string prefix="") {
		std::println("{0}: {1}.x = {2}, {1}.y = {3}",
			prefix, name, x, y);
	}
};

void foo::bar(int z) {
	x = z + y;
}

void func(foo k) {
	k.print("in func");
}

int main()
{
	auto f = foo("test");
	f.print();
	f.incr(3);
	f.print();
	f.bar(7);
	f.print();

	func(foo(5, 19));
	return 0;
}
