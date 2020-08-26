#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#ifdef __linux__
# include <pty.h>
# include <utmp.h>
#else
# include <util.h>
#endif

void
child(void)
{
	char *argv[] = { "/bin/ls", "-l", "/proc/self/fd", NULL };
	execv(argv[0], argv);
	err(EXIT_FAILURE, "execv %s", argv[0]);
}

int
main(int argc, char *argv[])
{
	int status;
	int fd[2]; /* primary/secondary fd of pty */
	(void)argv;

	if( argc > 1 ) {
		if( openpty(fd, fd + 1, NULL, NULL, NULL) ) {
			err(EXIT_FAILURE, "openpty");
		}
		switch( fork() ) {
		case -1:
			err(1, "fork");
			break;
		case 0:
			if( close(fd[0]) ) {
				err(EXIT_FAILURE, "close primary");
			}
			if( login_tty(fd[1]) ) {
				err(EXIT_FAILURE, "login_tty");
			}
			child(); /* Does not return */
		}
	} else {
		char name[PATH_MAX];
		switch( forkpty(fd, name, NULL, NULL) ) {
		case -1:
			err(1, "forkpty");
			break;
		case 0:
			child(); /* Does not return */
		}
	}
	wait(&status);
	if( WIFEXITED(status) && WEXITSTATUS(status) == 0 ) {
		char buf[BUFSIZ];
		ssize_t rc = read(fd[0], buf, sizeof buf);
		switch( rc ) {
		case -1:
			err(EXIT_FAILURE, "read");
		case 0:
			err(EXIT_FAILURE, "No data available\n");
		default:
			fwrite(buf, 1, rc, stdout);
		}
	} else if( WIFSIGNALED(status) ) {
		fprintf(stderr, "child caught signal %d\n", WTERMSIG(status));
	}
	return status;
}
