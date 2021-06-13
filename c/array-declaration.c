
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void)
{
	char w[] = "abcdefghijklmnop";
	char (*a)[5] = (void*)&w;
	char *b[5] = { "xyz", "Xar", "baz", "qux", "dog" };

	printf("char (*a)[] is a pointer to an array.\n");
	printf("char *b[] is an array of pointers.\n");

	printf( "*b[1] == b[1][0]: %c == %c\n", *(b[1]), b[1][0]);
	printf( "(*b)[1] == b[0][1]: %c == %c\n", (*b)[1], b[0][1]);

	printf( "*a[1] is UB: %c (expected to be f, but UB)\n", *a[1]);
	printf( "(*a)[1] is the 2nd element of *a: %c\n", (*a)[1]);
	return 0;
}
