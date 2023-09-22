
#include <string>
#include <utility>
#include <iostream>

class A {
	int member{};
public:
	~A() {}; // User defined destructor;
};

class B {
	int member{};
public:
	B(const B &obj) {} // User defined copy constructor
};

class C {
	int member{};
};

int main() {
	std::cout << "A: " << std::is_trivially_move_constructible<A>::value << std::endl;
	// A: 0 since A has a user-defined destructor.

	std::cout << "B: " << std::is_trivially_move_constructible<B>::value << std::endl;
	// B: 0 since B has a user-defined copy-constructor.

	std::cout << "C: " << std::is_trivially_move_constructible<C>::value << std::endl;
	// C: 1 all the rules for generating an implicit move are satisfied.
}

