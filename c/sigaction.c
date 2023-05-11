
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "xutil.h"

sig_atomic_t sig;

void
handle(int s, siginfo_t *i, void *v)
{
	(void)i;
	(void)v;
	sig = s;
	return;
}

int
main(void)
{
	struct sigaction act;

	printf("pid: %d\n", getpid());

	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	act.sa_flags = SA_RESTART; /* Not relevant here, just demonstrative */

	char *msg = "unknown signal";
	char *names[] = {
		[SIGUSR1] = "SIGUSR1",
		[SIGUSR2] = "SIGUSR2",
		[SIGHUP] = "SIGHUP",
		[SIGINT] = "SIGINT",
	};
	for( size_t i = 0; i < sizeof names / sizeof *names; i += 1 ){
		if( names[i] == NULL ){
			continue;
		}
		if( sigaction(i, &act, NULL ) ) {
			perror(names[i]);
			exit(1);
		}
	}
	pause();

	if( sig < sizeof names / sizeof *names && names[sig] ){
		msg = names[sig];
	}
	printf("received %s\n", msg);
	return 0;
}
