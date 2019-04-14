#define _POSIX_C_SOURCE 1
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>

void
hup(int sig)
{
	char msg[] = "parent exited\n";
	write(STDOUT_FILENO, msg, sizeof msg);
}

int
main(void)
{
	struct sigaction act = {0};
	pid_t parent = getpid();

	act.sa_handler = hup;
	sigemptyset(&act.sa_mask);
	sigaction(SIGHUP, &act, NULL);
	switch(fork()) {
	case -1: perror("fork"); exit(EXIT_FAILURE); break;
	case 0:
		if (prctl(PR_SET_PDEATHSIG, SIGHUP) == -1) {
			perror("prctl");
			exit(EXIT_FAILURE);
		}
		if(getppid() != parent) {
			fprintf(stderr, "parent exited before signal handling established\n");
			exit(EXIT_FAILURE);
		}
		pause();
		exit(EXIT_SUCCESS);
	default:
		sleep(1);
		exit(EXIT_SUCCESS);
	}
	return EXIT_FAILURE;
}
