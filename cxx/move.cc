// vim: noet ts=8 sw=8

#include <iostream>
#include <vector>

using namespace std;

class A {
public:
	int x;
	std::string name;
	A(int x, std::string name) : x{x}, name{name} {};
	A(A&& other) {
		cout << "move constructor: " << name << '\n';
		cout << "other: " << other.name << '\n';
	}
	A& operator=(A&& other) {
		cout << "move operator: " << name << '\n';
		cout << "other: " << other.name << '\n';
		return other;
	}

};

void foo(A a) {
	A b {5, "bar"};
	b = std::move(a);
	cout << "foo: " << a.name << '\n';
}
int main()
{
	A a { 5, "foo" };
	A b { std::move(a) };
	foo(std::move(a));
	return 0;
}
