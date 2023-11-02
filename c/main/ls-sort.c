
/* Execute ls | sort -r */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

static void xpipe(int *fd) { if( pipe(fd) == -1 ){ err(1, "pipe"); } }
static void xdup2(int a, int b) { if( dup2(a, b) == -1 ){ err(1, "dup2"); } }
static void xclose(int fd) { if( close(fd) == -1 ){ err(1, "close"); } }

static void
execute(int *fd, char **cmd, int w)
{
	switch( fork() ){
	case -1:
		err(EXIT_FAILURE, "fork");
	case 0:
		xdup2(fd[w], w);
		xclose(fd[0]);
		xclose(fd[1]);
		execvp(cmd[0], cmd);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
}

int
main(void)
{
	int rv = EXIT_SUCCESS;
	char *ls[] = { "ls", NULL };
	char *sort[] = { "sort", "-r", NULL };
	int fd[2];
	xpipe(fd);
	execute(fd, ls, 1);
	execute(fd, sort, 0);
	xclose(fd[0]);
	xclose(fd[1]);
	for( int i = 0; i < 2; i++ ){
		int status;
		wait(&status);
		if( ! WIFEXITED(status) || WEXITSTATUS(status) ){
			rv = EXIT_FAILURE;
		}
	}
	return rv;
}


