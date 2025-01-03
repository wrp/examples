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

sig_atomic_t stop, lap;
static void get_time(struct timeval *tp);
static void handle(int sig, siginfo_t *i, void *v);
static void establish_handlers(void);
static void print_delta(struct timeval begin, struct timeval end);

static void
show_lap(struct timeval now, struct timeval *prev)
{
	if (lap) {
		fputs("   ", stdout);
		print_delta(*prev, now);
		puts("         ");
		*prev = now;
		lap = 0;
	} else {
		fputs("                                 ", stdout);
	}

}


int
main(void)
{
	struct timeval start, now, prev;

	get_time(&start);
	prev = start;
	establish_handlers();

	while(!stop) {
		get_time(&now);
		putchar('\r');
		print_delta(start, now);
		show_lap(now, &prev);
		fflush(stdout);
		setlinebuf(stdout);
		pause();
	}
	putchar('\n');

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
	case SIGINT: lap = 1; break;
	case SIGQUIT: stop = 1; break;
	}
}


static void
establish_handlers(void)
{
	int to_catch[] = { SIGALRM, SIGINT, SIGQUIT };
	struct sigaction act;
	struct timeval tp = {.tv_sec = 0, .tv_usec = 100 };
	struct itimerval t = {.it_interval = tp, .it_value = tp };

	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	for (int i = 0; i < sizeof to_catch / sizeof *to_catch; i += 1) {
		if (sigaction(to_catch[i], &act, NULL)) {
			perror("sigaction");
			exit(1);
		}
	}
	setitimer(ITIMER_REAL, &t, NULL);
}


static void
print_delta(struct timeval begin, struct timeval end)
{
	struct timeval delta;
	timersub(&end, &begin, &delta);
	unsigned minutes = delta.tv_sec / 60;
	unsigned seconds = delta.tv_sec % 60;
	char usec[4];
	snprintf(usec, sizeof usec, "%u", delta.tv_usec);
	printf("%0um%02u.%ss", minutes, seconds, usec);
}
