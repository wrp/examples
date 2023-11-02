/* Check if a file descriptor is open without reading data from it. */

#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* int poll(struct pollfd fds[], nfds_t nfds, int timeout); */
int
main(void) {
	struct pollfd fd = { .fd = STDIN_FILENO, .events = POLLIN };
	if( poll(&fd, 1, 0) == -1 ){
		perror("poll");
		return EXIT_FAILURE;
	};
	if( fd.revents & POLLNVAL ){
		fprintf(stderr, "stdin is not open\n");
		return EXIT_FAILURE;
	} else {
		int c;
		while( (c = getchar()) != EOF ){
			putchar(c);
		}
	}
	return EXIT_SUCCESS;
}
