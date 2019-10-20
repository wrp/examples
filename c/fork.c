/* Demonstrate fork/exec */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void xclose(int fd);
void xdup2(int a, int b);
int
main(int argc, char **argv)
{
	int p1[2];
	int p2[2];
	int val = argc > 1 ? strtol(argv[1], NULL, 10) : 1;
	char buf[128];
	int len;
	ssize_t rc;
	if(pipe(p1) || pipe(p2)) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	switch(fork()) {
	case -1:
		perror("fork");
		return EXIT_FAILURE;
	case 0:
		xdup2(p1[0],STDIN_FILENO);
		xclose(p1[0]);
		xclose(p1[1]);
		xdup2(p2[1],STDOUT_FILENO);
		xclose(p2[0]);
		xclose(p2[1]);
		execlp("awk", "awk", "{print $1 * 2}", NULL);
		perror("exec");
		return EXIT_FAILURE;
	default:
		len = sprintf(buf, "%d", val);
		write(p1[1], buf, len);
		xclose(p1[1]);
		xclose(p1[0]);
		xclose(p2[1]);
		if(( rc = read(p2[0], buf, sizeof buf)) == -1) {
			perror("read");
		}
		xclose(p2[0]);
		buf[rc] = '\0';
		printf("%s", buf);
	}
	return EXIT_SUCCESS;
}
void
xclose(int fd) {
	if(close(fd)) {
		perror("close");
		exit(EXIT_FAILURE);
	}
}
void
xdup2(int a, int b) {
	if(dup2(a,b) == -1) {
		perror("dup2");
		exit(EXIT_FAILURE);
	}
}
