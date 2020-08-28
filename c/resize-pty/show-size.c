

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 600
#define _XOPEN_SOURCE_EXTENDED
#define _DARWIN_C_SOURCE

#include <err.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>

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
	while( (r = get_wch(&w)) != ERR ) {
		if( w == KEY_RESIZE ) {
			int y, x;
			getmaxyx(stdscr, y, x);
			printw("y = %d; x = %d\n", y, x);
			continue;
		} else if( w == KEY_UP ) {
			printw("keyup\n");
			continue;
		}
                char c[128];
		if( wctomb(c, w) == 1 && w >= 'a' && w <= 'z' ) {
			printw("%c", toupper(w));
		}
		doupdate();
	}
	endwin();
	return 0;
}
