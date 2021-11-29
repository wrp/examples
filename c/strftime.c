#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int
main(int argc, char **argv)
{
	int rv = EXIT_SUCCESS;
	struct timeval tv;
	struct tm *tm;
	char *default_args[] = { "%k:%M:%S %B %d, %Y", NULL };
	if( gettimeofday(&tv, NULL) == -1 ){
		perror("gettimeofday");
		exit(1);
	}
	argv = argc < 2 ? default_args : argv + 1;
	tm = gmtime(&tv.tv_sec);
	for( ; *argv; argv += 1 ){
		char buf[1024];
		const char *fmt = *argv;
		if( strftime(buf, sizeof buf, fmt, tm) ){
			printf("%s\n", buf);
		} else {
			fprintf(stderr, "Error formatting %s\n", fmt);
			rv = EXIT_FAILURE;
		}
	}
	return rv;
}

