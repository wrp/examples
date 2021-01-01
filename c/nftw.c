#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <ftw.h>
#include <unistd.h>

/* Call unlink or rmdir on the path, as appropriate. */
int
rm(const char *path, const struct stat *s, int flag, struct FTW *f)
{
	int status;
	int (*rm_func)(const char *);
	(void)s;
	(void)f;
	rm_func = flag == FTW_DP ? rmdir : unlink;
	if( status = rm_func(path), status != 0 ){
		perror(path);
	} else {
		puts(path);
	}
	return status;
}


int
main(int argc, char **argv)
{
	(void)argc;
	while( *++argv ) {
		if( nftw(*argv, rm, OPEN_MAX, FTW_DEPTH) ){
			perror(*argv);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
