
/* Execute ls | sort -r */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

static void xpipe(int *fd) { if( pipe(fd) == -1 ){ err(1, "pipe"); } }
static void xdup2(int a, int b) { if( dup2(a, b) == -1 ){ err(1, "dup2"); } }
static void xclose(int fd) { if( close(fd) == -1 ){ err(1, "close"); } }

int
main(void)
{
	char *ls[] = { "ls", NULL };
	char *sort[] = { "sort", "-r", NULL };
	int fd[2];
	xpipe(fd);
	switch( fork() ){
	case -1: perror("fork"); return EXIT_FAILURE;
	case 0:
		xdup2(fd[1], STDOUT_FILENO);
		xclose(fd[0]);
		xclose(fd[1]);
		execvp(ls[0], ls);
		perror("execvp");
		return EXIT_FAILURE;
	}
	switch( fork() ){
	case -1: perror("fork"); return EXIT_FAILURE;
	case 0:
		xdup2(fd[0], STDIN_FILENO);
		xclose(fd[0]);
		xclose(fd[1]);
		execvp(sort[0], sort);
		perror("execvp");
		return EXIT_FAILURE;
	}
}


