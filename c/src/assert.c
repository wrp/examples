

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int x = 5;
	assert(x == 4 && "Unexpected runtime value");
	_Static_assert(1, "This is how to do a compile time assertion");
	return 0;
}
