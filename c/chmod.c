#include "xutil.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>

/* Create a file with certain permissions (not modified by umask) */

int
main(int argc, char **argv)
{
	char *path = argc > 1 ? argv[1] : "file";
	int mode = argc > 2 ? strtol(argv[2], NULL, 8) : 0777;
	FILE *out;

	out = xfopen(path, "w+");
	chmod(path, mode);
	fprintf(out, "Hello, World!");
	return 0;
}
