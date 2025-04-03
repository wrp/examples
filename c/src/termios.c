/* From https://stackoverflow.com/questions/62895595 */

#include <stdio.h>
#include <string.h>
#include <termios.h>

static int
getch(void)
{
	struct termios term, oterm;
	tcgetattr(0, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &term);
	int c = getchar();
	tcsetattr(0, TCSANOW, &oterm);
	return c;
}

int
main(void)
{
	int c;
	puts("CTRL-C to exit");
	while( (c = getch()) != EOF ) {
		printf("%1$c (%1$x) pressed\n", c);
	}
	return 0;
}
