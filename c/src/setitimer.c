
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

void
handle(int sig, siginfo_t *i, void *v)
{
	(void)i;
	(void)v;
	(void) sig;
	struct timeval tp;
	char *date;
	if( gettimeofday(&tp, NULL ) == -1 ) {
		perror("gettimeofday");
		exit(1);
	}
	date = ctime(&tp.tv_sec);
	write(2, date, strlen(date));
}

int
main(void)
{
	struct sigaction act;
	struct timeval tp = {.tv_sec = 0, .tv_usec = 500000 };
	struct itimerval t = {.it_interval = tp, .it_value = tp };

	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	if( sigaction( SIGALRM, &act, NULL ) ) { perror("sigaction"); exit(1); }

	setitimer(ITIMER_REAL, &t, NULL);
	do pause(); while( errno == EINTR );

	return 0;
}
