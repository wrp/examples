/* Print a running stopwatch until SIGQUIT is received */


#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

sig_atomic_t stop, lap, reset;
static void get_time(struct timeval *tp);
static void handle(int sig, siginfo_t *i, void *v);
static void establish_handlers(void);
static void print_delta(struct timeval begin, struct timeval end);
static void check_user_activity(void);
static void show_lap(struct timeval, struct timeval *, struct timeval *);
static void hide_cursor() { system("tput vi"); }
static void show_cursor() { system("tput ve"); }
static void move_cursor_up_one_line() { system("tput cuu 1"); }
static void start_timer(void);


int
main(void)
{
	struct timeval start, now, prev;

	get_time(&start);
	puts("<return> for interval, ^c to reset, ^\\ to quit");
	hide_cursor();
	prev = start;
	establish_handlers();
	start_timer();

	while(!stop) {
		check_user_activity();
		get_time(&now);
		putchar('\r');
		print_delta(start, now);
		show_lap(now, &prev, &start);
		fflush(stdout);
	}
	show_cursor();
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
	case SIGINT: reset = 1; break;
	case SIGQUIT: stop = 1; break;
	}
}


static void
start_timer(void)
{
	struct timeval tp = {.tv_sec = 0, .tv_usec = 100 };
	struct itimerval t = {.it_interval = tp, .it_value = tp };
	setitimer(ITIMER_REAL, &t, NULL);
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


static void
print_delta(struct timeval begin, struct timeval end)
{
	struct timeval delta;
	timersub(&end, &begin, &delta);
	unsigned minutes = delta.tv_sec / 60;
	unsigned seconds = delta.tv_sec % 60;
	char usec[4];
	snprintf(usec, sizeof usec, "%ld", (long)delta.tv_usec);
	printf("%0um%02u.%ss", minutes, seconds, usec);
}


static void
check_user_activity(void)
{
	char b[128];
	while (-1 != read(STDIN_FILENO, b, 128)) {
		lap = 1;
	}
	if (lap) {
		move_cursor_up_one_line();
	}
}


static void
show_lap(struct timeval now, struct timeval *prev, struct timeval *start)
{
	if (lap || stop || reset) {
		fputs("   ", stdout);
		print_delta(*prev, now);
		putchar('\n');
		*prev = now;
		if (reset) {
			puts("*****");
			*start = now;
		}
		lap = reset = 0;
	}
}
