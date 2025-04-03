

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void
print_ints(int *x, size_t s)
{
	while( s-- ){
		printf("%d\n", *x++);
	}
}

int
main(int argc, char **argv)
{
	int explicit[] = {1, 2, 3};
	print_ints((int[]){1,2,3}, 3);
	print_ints(explicit, sizeof explicit / sizeof *explicit);
	return 0;
}
