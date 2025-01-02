/* Print a running stopwatch until SIGINT is received */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

sig_atomic_t stop;

static void
get_time(struct timeval *tp)
{
	if( gettimeofday(tp, NULL ) == -1 ) {
		perror("gettimeofday");
		exit(1);
	}
}

void
handle(int sig, siginfo_t *i, void *v)
{
	(void)i;
	(void)v;
	switch(sig) {
	case SIGINT: stop = 1; break;
	}
}

void
set_cursor_visibility(int on)
{
	if(on) {
		fputs("\x1b\x5b\x33\x34\x68\x1b\x5b\x3f\x32\x35\x68", stdout);
	} else {
		fputs("\x1b\x5b\x3f\x32\x35\x6c", stdout);
	}
}

int
main(void)
{
	struct sigaction act;
	struct timeval tp = {.tv_sec = 0, .tv_usec = 100 };
	struct itimerval t = {.it_interval = tp, .it_value = tp };
	struct timeval start, delta, now;

	set_cursor_visibility(0);
	get_time(&start);
	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	if( sigaction( SIGALRM, &act, NULL ) ) { perror("sigaction"); exit(1); }
	if( sigaction( SIGINT, &act, NULL ) ) { perror("sigaction"); exit(1); }
	if( sigaction( SIGQUIT, &act, NULL ) ) { perror("sigaction"); exit(1); }

	setitimer(ITIMER_REAL, &t, NULL);
	while(!stop) {
		get_time(&now);
		timersub(&now, &start, &delta);
		unsigned minutes = delta.tv_sec / 60;
		unsigned seconds = delta.tv_sec % 60;

		printf("%um%u.%us\033[K\r", minutes, seconds, delta.tv_usec);
		pause();
	}
	putchar('\n');
	set_cursor_visibility(1);

	return 0;
}
