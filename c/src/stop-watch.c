/* Print a running stopwatch until SIGQUIT is received */


#include <assert.h>
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
static void print_split(struct timeval, struct timeval, struct timeval);
static int print_delta(struct timeval, struct timeval, char *, size_t);
static void check_user_activity(void);
static void hide_cursor() { system("tput vi"); }
static void show_cursor() { system("tput ve"); }
static void start_timer(void);
static void to_col(int x) { printf("\r\e[%dC", x); }
static void save(void) { fputs("\e7", stdout); }
static void restore(void) { fputs("\e8", stdout); }
static void clear_to_end_of_line(void) { fputs("\e[K", stdout); }


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
	to_col(28);

	while(!stop) {
		check_user_activity();
		get_time(&now);
		save();
		print_split(prev, start, now);
		if (lap) {
			prev = now;
		}
		if (reset) {
			to_col(28);
			save();
			clear_to_end_of_line();
		}
		lap = reset = 0;
		restore();
		fflush(stdout);
	}
	show_cursor();
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
print_split(struct timeval prev, struct timeval start, struct timeval now)
{
	char buf[256];
	buf[0] = '\r';
	int len = 1;
	len += print_delta(start, now, buf + len, sizeof buf - len);
	len += snprintf(buf + len, sizeof buf - len, "   ");
	len += print_delta(prev, now, buf + len, sizeof buf - len);
	buf[len] = '\0';
	fputs(buf, stdout);
	fflush(stdout);
}

static int
print_delta(struct timeval begin, struct timeval end, char *buf, size_t siz)
{
	struct timeval delta;
	timersub(&end, &begin, &delta);
	unsigned minutes = delta.tv_sec / 60;
	unsigned seconds = delta.tv_sec % 60;
	char usec[4];
	snprintf(usec, sizeof usec, "%ld", (long)delta.tv_usec);
	return snprintf(buf, siz, "%0um%02u.%ss", minutes, seconds, usec);
}


static void
check_user_activity(void)
{
	char b[128];
	ssize_t n;
	while (-1 != (n = read(STDIN_FILENO, b, 127))) {
		lap = 1;
		to_col(28);
		save();
	}
}
