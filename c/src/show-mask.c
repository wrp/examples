/* Show the current set of blocked signals */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
	struct sigaction act = {{0}};
	sigset_t old;

	if(sigprocmask(SIG_SETMASK, NULL, &old)) {
		perror("sigprocmask");
		return EXIT_FAILURE;
	}
	printf("mask: %x\n", old);

	return !!old;
}
