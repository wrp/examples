#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
foo(void)
{
	printf("called from %s in %d\n", __func__, getpid());
	fflush(stdout);
}

void
bar(void)
{
	printf("called from %s in %d\n", __func__, getpid());
	fflush(stdout);
}

void
handle(int sig, siginfo_t *i, void *v)
{
	(void)i;
	(void)v;
	(void)sig;
	return;
}

int
main(void)
{
	struct sigaction act;

	/* Registered functions are not called for an uncaught signal */
	atexit(foo);
	atexit(bar);  /* Bar will be called first */


	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	if(sigaction( SIGUSR1, &act, NULL )) { perror("sigaction"); exit(1); }
	if(sigaction( SIGHUP, &act, NULL )) { perror("sigaction"); exit(1); }
	fork();
	printf("pid: %d\n", getpid());
	fflush(stdout);
	pause();
	return 0;
}
