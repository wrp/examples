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
		fputc(c, ofp) == EOF && die("%s", tmppath);
	}
	fflush(ofp) && die("fflush %s", tmppath);
	fsync(fileno(ofp)) && die("fsync %s", tmppath);
	fclose(ofp) && die("close %s", tmppath);
	rename(tmppath, path) && die("rename %s->%s", tmppath, path);
	return 0;
}
