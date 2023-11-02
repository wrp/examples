#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "xutil.h"

int
main(int argc, char **argv)
{
	int f = argc > 1 ? xopen( argv[1], O_RDONLY ) : STDIN_FILENO;
	while(1) {
		char b[1024];
		ssize_t rc;
		rc = read( f, b, sizeof b );
		switch(rc) {
		case 0:
			return EXIT_SUCCESS;
		case -1:
			perror("read");
			return EXIT_FAILURE;
		default:
			fwrite(b, 1, rc, stdout);
		}
	}
	return 0;
}
