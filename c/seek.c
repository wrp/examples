#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Seek into a file and write data at a given location. */

int
main( int argc, char **argv )
{
	int fd;
	int offset = argc > 1 ? strtol( argv[1], NULL, 0 ) : 0;
	char *string = argc > 2 ? argv[2] : "foo";
	char *path = argc > 3 ? argv[3] : "file";

	fd = open( path, O_RDWR | O_CREAT, 0777 );
	if( fd == -1 ) {
		perror( path );
		return 1;
	}
	if( lseek( fd, offset, SEEK_SET ) == -1 ) {
		perror( "lseek" );
		return 1;
	}
	if( write( fd, string, strlen( string )) == -1 ) {
		perror( "write" );
		return 1;
	}
	return 0;
}
