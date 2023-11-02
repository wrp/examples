/* Simple demonstration of fork */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>



static void
child(void){
	printf("child pid: %d\n", getpid());
	sleep(3);
	puts("Goodbye, world!");
	exit(7);
}

int
main(int argc, char **argv)
{
	int status;
	pid_t p = fork();

	switch( p ){
	case -1:
		perror("fork");
		return EXIT_FAILURE;
	case 0:
		child();
	default:
		if( -1 == waitpid(p, &status, 0) ){
			perror("waitpid");
			return EXIT_FAILURE;
		}
		if( WIFEXITED(status) ){
			printf("Child returned: %d\n", WEXITSTATUS(status));
		} else if( WIFSIGNALED(status) ){
			printf("Child signaled: %d\n", WTERMSIG(status));
		}
	}
}
