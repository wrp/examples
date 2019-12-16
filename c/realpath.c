#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <err.h>

void usage(void);

int
main(int argc, char **argv)
{
	char path[PATH_MAX];
	if( argc < 2 ) {
		err(EXIT_FAILURE, "No path specified");
	}
	if( realpath(argv[1], path) == NULL ) {
		perror(path);
	} else {
		printf("%s\n", path);
	}
	return 0;
}
