
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#if 1
#warning "This is a warning"
#else
#error "This is a fatal error"
#endif

int
main(int argc, char **argv)
{
	puts("Hello, World!");
	return 0;
}
