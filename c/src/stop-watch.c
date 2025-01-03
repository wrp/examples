/* Print a running stopwatch until SIGQUIT is received */

/*
** Instead of making the cursor invisible using fragile
** escape sequences, we try to keep the cursor out of the
** way by printing spaces.  This mostly works.  But if the
** user hits return and injects a newline into the terminal,
** things are wonky (ie, the cursor starts to be visible
** and jumps all over the line) until the next SIGINT is
** received to print an interval.  TODO: figure this out.
**/

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

	system("tput vi");
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
	system("tput ve");

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
