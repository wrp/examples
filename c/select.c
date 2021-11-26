/* Set a 10 second timer on a scanf */

#include <unistd.h>
#include <stdio.h>
int
main(void)
{
	struct timeval tp = {.tv_sec = 10, .tv_usec = 0 };
	char b[32];
	fd_set fds;

	FD_SET(STDIN_FILENO, &fds);
	switch( select(STDIN_FILENO + 1, &fds, NULL, NULL, &tp) ){
	case 1:
		if( FD_ISSET(STDIN_FILENO, &fds) ){
			if( scanf("%31s", b) == 1 ){
				printf("Read: %s\n", b);
			}
		}
		break;
	case 0:
		puts("Timeout");
		break;
	default:
		fputs("Error\n", stderr);
	}

}
