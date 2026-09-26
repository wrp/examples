
#include <iostream>
#include <stdexcept>

// Demonstrate the order in which destructors are called
// when an exception is thrown.

// A(0) constructed successfully
// A(1) constructed successfully
// A(1) destroyed
// B::B() exiting with exception
// A(0) destroyed
// C::C() exiting with exception
// main() failed to create C with: error initializing B::a2


struct A
{
	int n;

	A(int n = 0): n(n) {
		std::println("A({}) constructed successfully", n);
	}
	~A() { std::println("A({}) destroyed", n); }
};

int foo()
{
	throw std::runtime_error("error initializing B::a2");
}

struct B
{
	A a1, a2, a3;

	B() try : a1(1), a2(foo()), a3(3) {
		std::println("B constructed successfully");
	} catch(...) {
		std::println("B::B() exiting with exception");
	}
	~B() { std::println("B destroyed"); }
};

struct C : A, B
{
	C() try {
		std::println("C::C() completed successfully");
	} catch(...) {
		std::println("C::C() exiting with exception");
	}

	~C() { std::println("C destroyed"); }
};

int
main ()
try
{
    C c;
}
catch (const std::exception& e)
{
	std::println("main() failed to create C with: {}", e.what());
}
