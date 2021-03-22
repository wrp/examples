#include <ftw.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Search the entire filesystem for a file */

char *target;

int
find(const char *name, const struct stat *s, int flag, struct FTW *f)
{
	int status = 0;
	(void)s;
	(void)f;
	char buf[PATH_MAX];
	char *t = basename_r(name, buf);

	if( 1
		&& flag == FTW_F  /* Regular file */
		&& !access(name, X_OK)  /* executable */
		&& !strcmp(t, target) /* matches the name */
	) {
		puts(name);
	}
	return status;
}

int
main(int argc, char **argv)
{
	char *defaults[] = { NULL, argv[1], "/", NULL };

	switch( argc ){
	case 1: printf("usage: %s name [path ...]\n", basename(argv[0]));
		return EXIT_SUCCESS;
	case 2: argv = defaults;
	}
	target = argv[1];
	argv += 1;

	while( *++argv ) {
		if( nftw(*argv, find, OPEN_MAX, FTW_DEPTH) ){
			perror(*argv);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
