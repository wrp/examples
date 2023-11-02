/* Replace argv[1] with argv[2] */

#include <stdio.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>

void *
xmalloc(size_t s)
{
	void *rv = malloc(s);
	if(rv == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return rv;
}

int
main(int argc, char **argv)
{
	if( argc < 3 ) {
		errx(EXIT_FAILURE, "Insufficient arguments");
	}
	size_t len = strlen(argv[1]);
	int c;
	char *buf = xmalloc(len);
	char *s = buf;
	char *m = argv[1];
	while( (c = getchar()) != EOF ) {
		if( (*s++ = c) == *m ) {
			if( *++m == '\0' ) {
				fputs(argv[2], stdout);
				s = buf;
				m = argv[1];
			}
		} else {
			fwrite(buf, 1, s - buf, stdout);
			s = buf;
			m = argv[1];
		}
	}
	if( s != buf ) {
		fwrite(buf, 1, s - buf, stdout);
	}
	return 0;
}
