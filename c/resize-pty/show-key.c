
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 600
#define _XOPEN_SOURCE_EXTENDED
#define _DARWIN_C_SOURCE

#include <ctype.h>
#include <curses.h>
#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int xopen(const char *path, int flags);
void
handle(int sig, siginfo_t *i, void *v)
{
	(void)i;
	(void)v;
	char *n = NULL;
	switch(sig) {
	case SIGHUP:  n =  "SIGHUP\n"; break;
	case SIGTERM:  n = "SIGTERM\n"; break;
	case SIGINT:  n =  "SIGINT\n"; break;
	}
	if(n)
		write(2, n, strlen(n));
	return;
}

int
main(int argc, char **argv)
{
	int r;
	wint_t w = 0;
	if( argc > 1 ) {
		int fd = xopen(argv[1], O_WRONLY);
		dup2(fd, STDERR_FILENO);
	}
	unsetenv("COLUMNS");
	unsetenv("LINES");
	struct sigaction act;
	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	if( sigaction( SIGTERM, &act, NULL ) ) { perror("sigaction"); exit(1); }
	if( sigaction( SIGINT, &act, NULL ) ) { perror("sigaction"); exit(1); }
	if( sigaction( SIGHUP, &act, NULL ) ) { perror("sigaction"); exit(1); }
	if( initscr() == NULL ) {
		err(1, "initscr");
	}
	noecho();
	keypad(stdscr, true);
	while( (r = get_wch(&w)) != ERR ) {
		char *d = NULL;
		if( r == KEY_CODE_YES ) switch(w) {
		case KEY_RESIZE: d = "KEY_RESIZE"; break;
		case KEY_HOME: d = "KEY_HOME"; break;
		case KEY_END: d = "KEY_END"; break;
		case KEY_PPAGE: d = "KEY_PPAGE"; break;
		case KEY_NPAGE: d = "KEY_NPAGE"; break;
		case KEY_BACKSPACE: d = "KEY_BACKSPACE"; break;
		case KEY_DC: d = "KEY_DC"; break;
		case KEY_IC: d = "KEY_IC"; break;
		case KEY_BTAB: d = "KEY_BTAB"; break;
		case KEY_ENTER: d = "KEY_ENTER"; break;
		case KEY_UP: d = "KEY_UP"; break;
		case KEY_DOWN: d = "KEY_DOWN"; break;
		case KEY_RIGHT: d = "KEY_RIGHT"; break;
		case KEY_LEFT: d = "KEY_LEFT"; break;
		}
		if( d != NULL ) {
			/*printw("(%s)", d);*/
			fprintf(stderr, "%s", d);
		} else {
			/*printw("%lc", w);*/
			fprintf(stderr, "%lc", w);
		}
		/*doupdate();*/
		fflush(stderr);
	}
	endwin();
	fprintf(stderr, "ERR\n");
	return 0;
}

int
xopen(const char *path, int flags)
{
	int f = open(path, flags);
	if( f == -1 ) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return f;
}
