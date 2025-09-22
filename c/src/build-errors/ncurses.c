#include <err.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>

int
main(void)
{
	int r;
	int w = 0;
	unsetenv("COLUMNS");
	unsetenv("LINES");
	if( initscr() == NULL ) {
		err(1, "initscr");
	}
	printw("Type some characters, 'q' to quit\n");
	while( (w = getch()) != ERR ) {
		if (w == KEY_RESIZE) {
			int y, x;
			getmaxyx(stdscr, y, x);
			printw("y = %d; x = %d\n", y, x);
			continue;
		}
		if (w == 'q') {
			break;
		}
		if (w >= 'a' && w <= 'z') {
			printw("%c", toupper(w));
		}
		doupdate();
	}
	endwin();
	return 0;
}
