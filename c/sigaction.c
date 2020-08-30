
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "xutil.h"

sig_atomic_t usr1, usr2, hup;

void
handle(int sig, siginfo_t *i, void *v)
{
	(void)i;
	(void)v;
	switch(sig) {
	case SIGUSR1: usr1=1; break;
	case SIGUSR2: usr2=1; break;
	case SIGHUP: hup=1; break;
	}
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
	if( sigaction( SIGUSR1, &act, NULL ) ) { perror("sigaction"); exit(1); }
	if( sigaction( SIGUSR2, &act, NULL ) ) { perror("sigaction"); exit(1); }
	if( sigaction( SIGHUP, &act, NULL ) ) { perror("sigaction"); exit(1); }
	pause();

	printf("usr1=%d\n", usr1);
	printf("usr2=%d\n", usr2);
	printf("hup=%d\n", hup);
	return 0;
}
