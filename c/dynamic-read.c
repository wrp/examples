/* Read a text file into memory using read.  Reverse each line. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int xopen(const char *, int);
void * Realloc( void *buf, size_t s );
char * reverse_line( char *start);

int
main(int argc, char **argv)
{
	size_t rc;
	size_t file_size = 0;
	size_t siz = BUFSIZ;
	char *buf = Realloc(NULL, siz);
	char *line;
	int fd = argc > 1 ? xopen(argv[1], O_RDONLY) : STDIN_FILENO;

	/* Read the file into buf */
	while(( rc = read( fd, buf + file_size, BUFSIZ )) == BUFSIZ ) {
		file_size += rc;
		siz += BUFSIZ;
		buf = Realloc( buf, siz );
	}
	file_size += rc;
	buf[file_size]  = '\0';

	/* Reverse each line */
	line = buf;
	while( ( line = reverse_line(line)) != NULL )
		;
	fwrite( buf, 1, file_size, stdout );
}

char *
reverse_line( char *start)
{
	char *eol = strchr(start, '\n');
	char *rv = eol ? eol + 1 : NULL;
	if(eol) {
		for( eol -= 1; start < eol; start++, eol-- ) {
			char tmp = *eol;
			*eol = *start;
			*start = tmp;
		}
	}
	return rv;
}


void *
Realloc( void *buf, size_t s )
{
	buf = realloc( buf, s );
	if( buf == NULL) { perror("realloc"); exit(EXIT_FAILURE); }
	return buf;
}


int
xopen(const char *path, int flag)
{
	int fd = open(path, flag);
	if( fd == -1 ) { perror(path); exit(EXIT_FAILURE); }
	return fd;
}
