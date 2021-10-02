#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>



int
main(int argc, char **argv)
{
	struct tm tp = {0};
	char *end = "";
	time_t now = time(NULL);

	if( now == -1 ){
		perror("time");
	}

	int incr = argc > 1 ? strtol(argv[1], &end, 10) : 0;
	char *target = argc > 2 ? argv[2] : ctime(&now);
	char *date;

	if( target == NULL ){
		printf("usage: %s [offset [DD.MM.YYYY]]\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	if( *end || end == argv[1] ){
		fprintf(stderr, "Invalid offset\n");
		exit(EXIT_FAILURE);
	}
	end = strptime(target, "%d.%m.%Y", &tp);
	if( end == NULL ){
		end = strptime(target, "%a %b %d %H:%M:%S %Y", &tp);
	/*	                       Thu Nov 24 18:22:48 1986\n\0 */
	}
	if( end == NULL ){
		perror("strptime");
		exit(EXIT_FAILURE);
	}
	if( *end && *end != '\n'){
		fprintf(stderr, "Invalid date at '%c'\n", *end);
		exit(EXIT_FAILURE);
	}
	date = asctime(&tp);
	printf("%s", date);

	if( incr ){
		/* Convert struct tm to seconds from epoch */
		time_t tt = mktime(&tp);

		/* The call to mktime modifies tp, and relies on tp
		having been properly initialized */

		/* Increment days by adding seconds to epoch time */
		tt += incr * 24 * 3600;;
		printf("%s", ctime(&tt));

		/* Increment tm_mday and renormalize with mktime */
		tp.tm_mday += incr;
		tt = mktime(&tp);
		printf("%s%s", asctime(&tp), ctime(&tt));
	}
}
