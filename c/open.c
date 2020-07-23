#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>

/* Create a file with certain permissions (modified by umask, of course) */

int
main(int argc, char **argv)
{
	int fd;
	char *path = argc > 1 ? argv[1] : "file";
	int perm = argc > 2 ? strtol(argv[2], NULL, 8) : 0777;
	FILE *out;

	fd = open(path, O_RDWR | O_CREAT, perm);
	if( fd == -1 ) {
		err(1, "%s", path);
	}
	out = fdopen(fd, "w");
	fprintf(out, "Hello, World!");
	return 0;
}
