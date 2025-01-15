/* Simple demonstration of parent receiving SIGCHLD
** In particular, demonstrate that the parent receives
** SIGCHLD when the child STOPS
*/


#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


static void child(int x);
static pid_t make_child(int);
static void establish_handler(int);
static void check_status(pid_t p);


void
handle(int s, siginfo_t *i, void *v)
{
	write(STDERR_FILENO, "SIGCHLD\n", 8);
	return;
}


int
main(int argc, char **argv)
{
	pid_t p[3];
	establish_handler(0);
	p[0] = make_child(0);
	p[1] = make_child(1);
	p[2] = make_child(2);

	check_status(p[0]);

	printf("Stopping %d\n", (int)p[1]);
	kill(p[1], SIGSTOP);
	printf("continuing %d\n", (int)p[1]);
	kill(p[1], SIGCONT);
	printf("Terminating %d\n", (int)p[1]);
	kill(p[1], SIGTERM);
	check_status(p[1]);

	printf("Re-establising handler with SA_NOCLDSTOP\n");
	establish_handler(1);
	printf("Stopping %d\n", (int)p[2]);
	kill(p[2], SIGSTOP);
	printf("continuing %d\n", (int)p[2]);
	kill(p[2], SIGCONT);

	printf("Terminating %d\n", (int)p[2]);
	kill(p[2], SIGINT);
	check_status(p[2]);
}

static void
check_status(pid_t p)
{
	int status;
	printf("Checking %d\n", (int)p);
	fflush(stdout);
	while (-1 == waitpid(p, &status, 0)) {
		if (errno != EINTR) {
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
	if (WIFEXITED(status)) {
		printf("Child %d returned: %d\n", (int)p, WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("Child %d signaled: %d\n", (int)p, WTERMSIG(status));
	}
}


static pid_t
make_child(int x)
{
	pid_t p = fork();
	if (p == -1) {
		perror("fork");
		exit(1);
	}
	if (p == 0) {
		child(x);
	}
	return p;
}


static void
child(int x)
{
	switch (x) {
	case 0: _exit(0);
	}
	pause();
	printf("Child %d unpaused\n", x);
	fflush(stdout);
	_exit(7);
}

static void
establish_handler(int x)
{
	struct sigaction act;

	memset(&act, 0, sizeof act);
	act.sa_sigaction = handle;
	act.sa_flags = x ? SA_NOCLDSTOP : 0;

	/* SA_NOCLDSTOP    If this bit is set when installing a
	** catching function for the SIGCHLD signal, the SIGCHLD
	** signal will be generated only when a child process exits,
	** not when a child process stops.
	*/

	if (sigaction(SIGCHLD, &act, NULL)) {
		perror("sigaction");
		exit(1);
	}
}
