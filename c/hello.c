#include <stdio.h>

int
main(int argc, char *const argv[])
{
	char *defaults[] = { "World", NULL };
	argv = argc > 1 ? argv + 1 : defaults;
	printf("Hello, %s!\n", argv[0]);
	return 0;
}
