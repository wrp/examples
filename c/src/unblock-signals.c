/* Interesting!  I am currently in the state where new shells seem
 * to be ignoring SIGPIPE.  The symptom is that when I start a new
 * shell and run 'yes | head', it hangs.  I suspect yes is inheriting
 * the disposition and ignoring SIGPIPE.  This probably has to do with
 * the version of tmux I'm running.  So this is a simple wrapper
 * to reset the disposition and run a command.
 */

#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void
set_default(struct sigaction *act, int s)
{
	if(sigaction(s, act, NULL)) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

int
main(int argc, char **argv)
{
	struct sigaction act = {{0}};
	sigset_t new;

	sigemptyset(&new);

	if(sigprocmask(SIG_SETMASK, &new, NULL)) {
		perror("sigprocmask");
		return EXIT_FAILURE;
	}

	act.sa_handler = SIG_DFL;
	set_default(&act, SIGPIPE);
	set_default(&act, SIGINT);
	set_default(&act, SIGTERM);

	execvp(argv[1], argv+1);

	perror("execlp");
	return EXIT_FAILURE;
}
