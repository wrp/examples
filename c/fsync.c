#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include "xutil.h"

/* Simple description of fsync/rename.
 * Consume stdin and write to a file.
 */

int
main(int argc, char **argv)
{
	if( argc != 2 ) {
		printf("usage: %s output-file\n", basename(argv[0]));
		puts("\nCopy stdin to output-file");
		return EXIT_SUCCESS;
	}
	char *path = argv[1];
	char *tmppath = xmalloc(strlen(path) + 5);
	sprintf(tmppath, "%s.tmp", path);
	FILE *ofp = xfopen(tmppath, "w");
	int c;

	while( (c = getchar()) != EOF) {
		if(fputc(c, ofp) == EOF) {
			die("%s", tmppath);
		}
	}
	fflush(ofp) && die("%s", "fflush");
	fsync(fileno(ofp)) && die("%s", "fsync");
	fclose(ofp) && die("%s", "close");
	rename(tmppath, path) && die("%s", path);
	return 0;
}
