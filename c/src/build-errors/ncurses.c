#include <err.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>


int
main(void)
{
	int r;
	int w = 0;
	int x = 0, y = 0;
	int *this = &x;

	unsetenv("COLUMNS");
	unsetenv("LINES");
	if( initscr() == NULL ) {
		err(1, "initscr");
	}
	cbreak();
	curs_set(0);  /* cursor visiblity 0, 1, 2 */
	/* noecho(); */
	keypad(stdscr, TRUE);
	/* nodelay(stdscr, TRUE); */
	printw("Type some characters, 'Q' to quit\n");
	while( (w = getch()) != ERR ) {
		if (w == KEY_RESIZE) {
			int a, b;
			getmaxyx(stdscr, a, b);
			printw("y = %d; x = %d\n", a, b);
			continue;
		}
		switch(w){
		case 'b': beep(); break;
		case 'f': flash(); break;
		case 'h': attron(A_REVERSE); break;
		case 'o': attroff(A_REVERSE); break;
		case 'H': attron(A_STANDOUT); break;
		case 'O': attroff(A_STANDOUT); break;
		case 'u': attron(A_UNDERLINE); break;
		case 'U': attroff(A_UNDERLINE); break;
		case 'Q': goto fini;
		case 'g':
			printw("Moving to (%d, %d)", y, x);
			move(y, x);
			/* fall thru */
		case ',':
			this = this == &x ? &y : &x;
			*this = 0;
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			*this = 10 * *this + (w - '0');
			break;
		default: printw("%c", toupper(w));
		}
		doupdate();
	}
fini:
	endwin();
	return 0;
}
