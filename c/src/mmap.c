/* Simple example using mmap to cat a regular file */
#include "xutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

size_t
getsize(int fd) {
	struct stat st;
        if( fstat(fd, &st) == -1 ) {
		perror("fstat");
		exit(EXIT_FAILURE);
	}
	/* If fd is a pipe, mmap will fail */
	return st.st_size;
}

int
main(int argc, char **argv)
{
	int fd = argc > 1 ? xopen(argv[1], O_RDONLY) : STDIN_FILENO;
	size_t s = getsize(fd);
	char *d = mmap(NULL, s, PROT_READ, MAP_PRIVATE
#if HAVE_DECL_MAP_POPULATE
	| MAP_POPULATE
#endif
	, fd, 0);
	if( d==MAP_FAILED ) {
		perror("mmap");
		return EXIT_FAILURE;
	}
	fwrite(d, 1, s, stdout);
	return 0;
}
