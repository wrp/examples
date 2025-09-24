#include <assert.h>
#include <curses.h>
#include <ctype.h>

#define CTRL(x) ((x) & 0x1f)

int
main(void)
{
	initscr();
	noecho(); // Instruct terminal driver to not print keys
	curs_set(0); // Hide the cursor
	mvprintw(0, 0, "Press any key (q to quit)");
	refresh();

	timeout(1000); /* Cause getch to return ERR after 1000 ms */
	for (;;) {
		int c = getch();

		move(2, 11);
		if (c == 'q') {
			break;
		}
		printw("'%s' (0x%02x)", keyname(c), c);
		clrtoeol();
		refresh();
	}

	endwin();
	return 0;
}
