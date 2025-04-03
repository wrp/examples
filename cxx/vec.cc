
#include <iostream>
#include <vector>

using namespace std;

struct Foo {
	int x;
	Foo(int x) : x{x} { cerr << "ctor: " << x << endl; };
	Foo(const Foo &f) : x{f.x} { cerr << "cp: " << f.x << endl; };
	~Foo() { cerr << "destroy: " << x << endl; }
};


int main()
{
	vector<Foo> x{};
	x.reserve(10);
	x.emplace_back(1);
	x.emplace_back(2);
	x.emplace_back(3);
}
