

#include <iostream>
#include <vector>

using namespace std;

class foo {
public:
	double x;
	double y;

	foo() : x{1.0}, y{2.0} {};
	foo(double x, double y) : x{x}, y{y} {}

	void incr(double a) { x += a; y -= a; }
	void bar(int);
};

void foo::bar(int z) {
	x = z + y;
}

void func(foo k) {
	cout << "in f, x = " << k.x << " y = " << k.y << endl;
}

int main()
{
	auto f = foo();
	cout << f.x << endl;
	cout << f.y << endl;
	f.incr(3);
	cout << f.x << endl;
	cout << f.y << endl;

	f.bar(7);
	cout << f.x << endl;
	cout << f.y << endl;

	func(foo(5, 19));
	return 0;
}
