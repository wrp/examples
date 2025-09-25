#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <stdio.h>

int
main(void)
{
	int err;

	if (setupterm(NULL, STDOUT_FILENO, &err) != OK) {
		return 1;
	}

	const char *smul = tigetstr("smul");   // enter underline
	const char *rmul = tigetstr("rmul");   // exit underline
	if (smul && rmul) {
		tputs(smul, 1, putchar);
		printf("underlined\n");
		tputs(rmul, 1, putchar);
	} else {
		// fallback if underline unsupported: use standout
		tputs(tigetstr("smso"), 1, putchar);
		printf("fallback\n");
		tputs(tigetstr("rmso"), 1, putchar);
	}
}
