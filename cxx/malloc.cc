

// IMO, malloc should *never* be used in C++.  But if you do
// use it, use it with the proper cast:

#include <cstdlib>

int
main()
{
	int n = 5;
	int *x = static_cast<decltype(x)>(malloc(n * sizeof *x));
	return 0;
}
