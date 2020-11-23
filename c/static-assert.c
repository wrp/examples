#include <stdio.h>

int
main(void)
{
	char str[10] = "Hello";
_Static_assert( sizeof str == 15, "Invalid size");
	printf("%s!\n", str);
	return 0;
}
