
#include <cassert>
#include <string>
#include <utility>
#include <iostream>

class A {
public:
	~A() {}; // User defined destructor;
};

class B {
public:
	B(const B &obj) {} // User defined copy constructor
};

class C {
};

int main() {
	// False since A has a user-defined destructor.
	assert(! std::is_trivially_move_constructible<A>::value);

	// False since B has a user-defined copy-constructor.
	assert(! std::is_trivially_move_constructible<B>::value);

	// True: all the rules for generating an implicit move are satisfied.
	assert(std::is_trivially_move_constructible<C>::value);
}

