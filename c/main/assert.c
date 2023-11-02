

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	_Static_assert(1, "This is how to do a compile time assertion");
	return 0;
}
