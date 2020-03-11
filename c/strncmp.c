#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main(int argc, char **argv)
{
	const char *cmp;
	size_t n;
	if(argc < 3) {
		printf("usage: %s N cmp [string...]\n", basename(argv[0]));
		puts("\nShow the result of strncmp(cmp, string, N) on each of the strings");
		return 0;
	}
	n = strtol(argv[1], NULL, 10);
	cmp = argv[2];
	argv += 2;
	for(; *argv; argv += 1) {
		printf("strncmp(\"%s\", \"%s\", %zu) = %d\n",
			cmp, *argv, n, strncmp(cmp, *argv, n));
	}
	return 0;
}
