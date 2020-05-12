#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
foo(void)
{
	char *buf = "called from foo\n";
	write(1, buf, strlen(buf));
}

void
bar(void)
{
	printf("called from %s\n", __func__);
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

	printf("pid: %d\n", getpid());

	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	if(sigaction( SIGUSR1, &act, NULL )) { perror("sigaction"); exit(1); }
	if(sigaction( SIGHUP, &act, NULL )) { perror("sigaction"); exit(1); }
	pause();
	return 0;
}
