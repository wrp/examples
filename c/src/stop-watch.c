/* Print a running stopwatch until SIGINT is received */

/* TODO
   currently, this ignores keypresses so the user can
   insert a newline by hitting 'return', which is
   nice behavior.  It would be nice to instead grab
   the keystroke and print intervals.

   Also, need to get the raw ansi escape sequences
   out and actually query to terminfo data base.  Maybe
   using SIGINT to print laptimes and abort on SIGQUIT
*/

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

sig_atomic_t stop;
static void get_time(struct timeval *tp);
static void handle(int sig, siginfo_t *i, void *v);
static void set_cursor_visibility(int on);
static void establish_handlers(void);


int
main(void)
{
	struct timeval tp = {.tv_sec = 0, .tv_usec = 100 };
	struct itimerval t = {.it_interval = tp, .it_value = tp };
	struct timeval start, delta, now;

	set_cursor_visibility(0);
	get_time(&start);
	establish_handlers();


	setitimer(ITIMER_REAL, &t, NULL);
	while(!stop) {
		get_time(&now);
		timersub(&now, &start, &delta);
		unsigned minutes = delta.tv_sec / 60;
		unsigned seconds = delta.tv_sec % 60;

		char usec[4];
		snprintf(usec, sizeof usec, "%u", delta.tv_usec);

		printf("%0um%02u.%ss\033[K\r", minutes, seconds, usec);
		pause();
	}
	putchar('\n');
	set_cursor_visibility(1);

	return 0;
}


static void
get_time(struct timeval *tp)
{
	if( gettimeofday(tp, NULL ) == -1 ) {
		perror("gettimeofday");
		exit(1);
	}
}


static void
handle(int sig, siginfo_t *i, void *v)
{
	(void)i;
	(void)v;
	switch(sig) {
	case SIGINT: stop = 1; break;
	}
}


static void
set_cursor_visibility(int on)
{
	if(on) {
		fputs("\x1b\x5b\x33\x34\x68\x1b\x5b\x3f\x32\x35\x68", stdout);
	} else {
		fputs("\x1b\x5b\x3f\x32\x35\x6c", stdout);
	}
}


static void
establish_handlers(void)
{
	int to_catch[] = { SIGALRM, SIGINT, SIGQUIT };
	struct sigaction act;

	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	for (int i = 0; i < sizeof to_catch / sizeof *to_catch; i += 1) {
		if (sigaction(to_catch[i], &act, NULL)) {
			perror("sigaction");
			exit(1);
		}
	}
}
