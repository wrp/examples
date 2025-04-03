

#include <iostream>
#include <vector>

using namespace std;

class foo {
public:
	double x, y;

	foo() : x{1.0}, y{2.0} {};
    foo(double x, double y) : x{x}, y{y} {}

	void incr(double a) { x += a; y -= a; }
	void bar(int);
};

void foo::bar(int z){
	x = z + y;
}

void func(foo k) {
	cout << "in f, x = " << k.x << " y = " << k.y << endl;
}

int main()
{
	auto f = foo();
	std::cout << f.x << std::endl;
	std::cout << f.y << std::endl;
	f.incr(3);
	std::cout << f.x << std::endl;
	std::cout << f.y << std::endl;

	f.bar(7);
	std::cout << f.x << std::endl;
	std::cout << f.y << std::endl;

	func(foo(5, 19));
	return 0;
}
