#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int
main(int argc, char *argv[])
{
	char *template = argc > 1 ? argv[1] : "mkstemp-example.XXXXXX";
	char tempname[PATH_MAX];
	int fd;
	char *tmpdir = getenv("TMPDIR");
	int length = 0;

	if( tmpdir ) {
		length = snprintf(tempname, sizeof tempname - 1, "%s", tmpdir);
		if( tempname[length - 1] != '/' ) {
			tempname[length++] = '/';
		}
	}
	tempname[length] = '\0';

	strncat(tempname, template, sizeof tempname - length);

	printf("name: %s\n", tempname);
	if ((fd = mkstemp(tempname)) == -1) {
		perror("mkstemp");
		return 1;
	}
	printf("name: %s\n", tempname);
	unlink(tempname);

	return EXIT_SUCCESS;
}
