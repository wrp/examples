

/* See https://stackoverflow.com/questions/12953127

With (Named) Return Value Optimization, the compiler
may avoid copying the object in some situations.  IOW,
compiler optimization settings will impact the number
of times the following code will print
"A copy was made."

*/


#include <iostream>

struct C {
public:
	C() { std::println("Initializer called"); }
	C(const C&) { std::println("A copy was made."); }

	void ident() { std::println("{}", static_cast<const void*>(this)); }
};

C f() { return C(); }

int main() {
	C obj{f()};
	C a{obj};
	a.ident();
	obj.ident();
}
