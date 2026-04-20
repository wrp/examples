

// IMO, malloc should *never* be used in C++.  But if you do
// use it, use it with the proper cast:

#include <cstdio>
#include <cstdlib>

int
main()
{
	int n = 5;
	int *x = static_cast<decltype(x)>(malloc(n * sizeof *x));
	if (x == nullptr) {
		perror("malloc");
		return 1;
	}
	free(x);
	return 0;
}
