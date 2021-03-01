

/* See https://stackoverflow.com/questions/12953127

With (Named) Return Value Optimization, the compiler
may avoid copying the object in some situations.  IOW,
compiler optimization settings will impact the number
of times the following code will print
"A copy was made."

*/


#include<iostream>
using namespace std;

struct C {
	C() { std::cout << "Initializer called" << endl; }
	C(const C&) { std::cout << "A copy was made." << endl; }
};

C f() {
	return C();
}

int main() {
	C obj = f();
}
