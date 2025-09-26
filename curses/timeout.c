#include <assert.h>
#include <ctype.h>
#include <curses.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

volatile sig_atomic_t sig;

typedef void (*handler)(int, siginfo_t *, void *);

static void set_timers(unsigned delay_msec);
static char * get_time(void);
static void set_handlers(handler);

void
handle(int s, siginfo_t *i, void *v) {
	(void)i; (void)v;
	sig = s;
}

int
main(void)
{
	set_handlers(handle);
	set_timers(2000);
	initscr();
	noecho(); // Instruct terminal driver to not print keys
	curs_set(0); // Hide the cursor
	mvprintw(0, 0, "Press any key (q to quit)");
	refresh();

	timeout(787); /* Cause getch to return ERR after 787 ms */
	for (;;) {
		int c = getch();

		if (c == 'q') {
			break;
		}
		mvprintw(2, 11, "'%s' (0x%02x)", keyname(c), c);

		if (sig) {
			mvprintw(3, 11, "   sig at %s", get_time());
			sig = 0;
		} else {
			mvprintw(4, 11, "no sig at %s", get_time());
		}
		clrtoeol();
		refresh();
	}

	endwin();
	return 0;
}



static void
set_handlers(handler h)
{
	struct sigaction act;

	memset(&act, 0, sizeof act);
	act.sa_sigaction = h;
	if( sigaction( SIGALRM, &act, NULL ) ) {
		perror("sigaction");
		exit(1);
	}
}

static void
set_timers(unsigned delay_msec)
{
	long sec = 0;
	while( delay_msec > 999 ) {
		sec += 1;
		delay_msec -= 1000;
	}
	struct timeval tp = {.tv_sec = sec, .tv_usec = delay_msec * 1000};
	struct itimerval t = {.it_interval = tp, .it_value = tp };
	setitimer(ITIMER_REAL, &t, NULL);
}

static char *
get_time(void)
{
	static char buf[64];
	struct timeval tp;
	if( gettimeofday(&tp, NULL ) == -1 ) {
		perror("gettimeofday");
		exit(1);
	}
	ctime_r(&tp.tv_sec, buf);
	snprintf(buf + 19, 40, ".%d", tp.tv_usec);
	return buf;
}
