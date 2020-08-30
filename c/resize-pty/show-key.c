
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 600
#define _XOPEN_SOURCE_EXTENDED
#define _DARWIN_C_SOURCE

#include <err.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

int
main(void)
{
	int r;
	wint_t w = 0;
	unsetenv("COLUMNS");
	unsetenv("LINES");
	if( initscr() == NULL ) {
		err(1, "initscr");
	}
	noecho();
	keypad(stdscr, true);
	while( (r = get_wch(&w)) != ERR ) {
		char s[64];
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
			sprintf(s, "(%s)", d);
		} else {
			sprintf(s, "%lc", w);
		}
		printw("%s", s);
		doupdate();
	}
	fprintf(stderr, "ERR");
	endwin();
	return 0;
}
