/* Simple example the uses mmap to cat a regular file */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
xopen(const char *path, int flags)
{
	int f = open(path, flags);
	if( f == -1 ) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return f;
}

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
	int fd = argv[1] ? xopen(argv[1], O_RDONLY) : STDIN_FILENO;
	size_t s = getsize(fd);
	char *d = mmap(NULL, s, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
	if( d==MAP_FAILED ) {
		perror("mmap");
		return EXIT_FAILURE;
	}
	fwrite(d, 1, s, stdout);
	return 0;
}
