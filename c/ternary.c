#include <stdio.h>
#include <stdlib.h>

/* Nice (?) technique allowing use of ternary operator on the LHS */
int
main(int argc, char **argv)
{
	int x = argc > 1 ? strtol(argv[1], NULL, 10 ) : 1;
	int y = argc > 2 ? strtol(argv[2], NULL, 10 ) : 1;

	*(x > y ? &x : &y) = 5;

	printf("x = %d, y = %d\n", x, y);
	return 0;
}
