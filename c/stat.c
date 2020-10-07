/* Print the user id of the owner of a file */

#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>

int
main( int argc, char **argv )
{
	struct stat sb;
	struct passwd pwent;
	struct passwd *pwentp;
	char buf[ _SC_GETPW_R_SIZE_MAX ];

	char *filepath = argc > 1 ? argv[1] : "foo";

	if( stat( filepath, &sb ) == -1 ) {
		perror( "stat" );
		return EXIT_FAILURE;
	}

	if( !getpwuid_r( sb.st_uid, &pwent, buf, sizeof buf, &pwentp )) {
		printf( "%s\n", pwent.pw_name );
	} else {
		printf( "%d\n", sb.st_uid );
	}
}
