

#include <assert.h>

using namespace std;


void foo(int& b) { b += 1; }

int
main()
{
	// Demonstrate major failing of the language
	auto a{5};
	assert( a == 5 );
	foo(a);
	assert( a == 6 );

	// The user cannot tell by inspecting the call
	// site whether the semantics are pass by
	// reference or pass by value.  You *must*
	// use external information (eg, you must inspect the
	// definition of foo) to know whether
	// or not arguments are modified.  This
	// is a source of great fragility.
	return 0;
}
