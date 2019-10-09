/* simple example the writes and catches sigpipe and sighup */

#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void
handle(int s, siginfo_t *i, void *v)
{
	char buf[32];
	int len;
	(void)i;
	(void)v;
	len = sprintf( buf, "handle: %d\n", s );
	write(2, buf, len);
	_exit(0);
	return;
}

void
set_action(struct sigaction *act, int s)
{
	if(sigaction(s, act, NULL)) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}
int
main(void)
{
	int i = 0;
	char buf[32];
	struct sigaction act = {{0}};

	act.sa_sigaction = handle;

	printf( "pipe catcher %d\n", getpid());
	set_action(&act, SIGPIPE);
	set_action(&act, SIGHUP);
	set_action(&act, SIGINT);
	while(1) {
		sprintf(buf, "%d\n", i++);
		write(1, buf, strlen(buf));
		sleep(1);
	}

	return 0;
}
