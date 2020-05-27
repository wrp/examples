#include <stdio.h>
#include <stdlib.h>

/* Nice (?) technique allowing use of ternary operator on the LHS */

struct foo {
	int x;
};
int
main(int argc, char **argv)
{
	int x = argc > 1 ? strtol(argv[1], NULL, 10 ) : 1;
	int y = argc > 2 ? strtol(argv[2], NULL, 10 ) : 1;
	struct foo f = {0}, g = {0};

	*(x > y ? &x : &y) = 5;
	(x == 5 ? &f : &g)->x = 6;

	printf("x = %d, y = %d\n", x, y);
	printf("f.x = %d, g.x = %d\n", f.x, g.x);
	return 0;
}
