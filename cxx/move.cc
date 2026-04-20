#include <iostream>
#include <vector>

using namespace std;

class A {
public:
	int x;
	std::string name;
	A(int x, std::string name) : x{x}, name{name} {
		cout << "regular ctor: " << name << '\n';
	}
	A(A&& other) : x{other.x}, name{other.name} {
		cout << "move constructor: " << name << '\n';
		cout << "other: " << other.name << '\n';
	}
	A(A& other) : x{other.x}, name{other.name} {
		cout << "copy constructor: " << name << '\n';
		cout << "other: " << other.name << '\n';
	}
	A& operator=(A&& other) {
		cout << "move operator: " << name << '\n';
		cout << "other: " << other.name << '\n';
		x = other.x;
		name = other.name;
		return *this;
	}

};

class no_move_ctor {
public:
	int i;
	no_move_ctor(int a) : i{a} { };
	no_move_ctor(const no_move_ctor& other) {
		cout << "copy constructor: " << (i = other.i) << '\n';
	}
};

void foo(A a) {
	cout << "in foo: " << a.name << '\n';
	A b {5, "bar"};
	b = std::move(a);
	cout << "foo: " << b.name << '\n';
}
int main()
{
	A a { 5, "foo" };
	A b { std::move(a) };
	cout << "main: " << b.name << '\n';
	foo(b);

	const no_move_ctor c{0};
	auto d = std::move(c);
	// auto d = c;
	return 0;
}
