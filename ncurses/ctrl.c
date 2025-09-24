#include <assert.h>
#include <curses.h>
#include <ctype.h>

#define CTRL(x) ((x) & 0x1f)

int
main(void)
{
	initscr();
	raw();    // catch ^C, ^Z, etc.  Use cbreak() to let them pass
	noecho(); // Instruct terminal driver to not print keys
	curs_set(0); // Hide the cursor
	keypad(stdscr, TRUE); // decode arrows/F-keys to KEY_* codes

	mvprintw(0, 0, "Press keys (q to quit)");
	refresh();

	for (;;) {
		int c = getch();

		if (c == 'q') {
			break;
		}
		mvprintw(2, 0, "Got ");
		if (c >= 1 && c <= 26) {
			int base = 'A' + (c - 1);
			assert(base == (c | 0x40));
			printw("Ctrl-%c (0x%02x)", base, c);
		} else if (c >= KEY_MIN) {
			// Function/arrow keys become KEY_* constants
			printw("%s", keyname(c));
		} else {
			printw("'%s' (0x%02x)", keyname(c), c);
		}
		clrtoeol();
		refresh();
	}

	endwin();
	return 0;
}
