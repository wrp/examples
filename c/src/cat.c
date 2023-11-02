#include "xutil.h"
#include <err.h>
#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
handle(int sig, siginfo_t *i, void *v)
{
	(void)i;
	(void)v;
	char buf[128];
	sprintf(buf, "Received: %d\n", sig);
	write(2, buf, strlen(buf));
	exit(1);
}

int
main(int argc, char **argv)
{
	struct sigaction act;

	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	if( sigaction( SIGPIPE, &act, NULL ) ) { perror("sigaction"); exit(1); }

	char *const defaults[] = { "-", NULL };
	char *const*args = argc > 1 ? argv + 1: defaults;

	if( argv[1] && !strcmp(argv[1], "-h")) {
		printf("usage: %s [path ...]\n", basename(argv[0]));
		exit(EXIT_SUCCESS);
	}
	for( ; *args; args++ ){
		int c;
		FILE *in = xfopen(*args, "r");
		if( argc > 2 ) {
			printf("******** %s ********\n", *args);
		}
		while( ( c = fgetc(in)) != EOF ) {
			putchar(c);
			fflush(stdout);
		}
		if(fclose(in)) {
			err(EXIT_FAILURE, "close %s", *args);
		}
	}
}
