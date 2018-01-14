
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/prctl.h>

/* Demonstrate use of prctl to get a signal when the parent terminates */
void
handle(int s, siginfo_t *i, void *v)
{
	write(1, "handle\n", 7);
	return;
}

int
main(void)
{
	struct sigaction act = {{0}};

	act.sa_sigaction = handle;

	printf("%d\n", getpid());
	if(sigaction( SIGUSR1, &act, NULL )) {
		perror("sigaction");
		exit(1);
	}

	/* We'll get a SIGUSR1 when our parent terminates */
	if( prctl( PR_SET_PDEATHSIG, SIGUSR1 )) {
		perror("prctl");
		exit(1);
	}
	pause();
	return 0;
}
