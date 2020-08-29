#include <ctype.h>
#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <unistd.h>
#ifdef __linux__
# include <pty.h>
# include <utmp.h>
#else
# include <util.h>
#endif

static ssize_t
send_msg(int fd, const char *msg)
{
	char buf[1024];
	if( msg ) {
		write(fd, msg, strlen(msg));
	}
	ssize_t s = read(fd, buf, sizeof buf);
	const char *t = buf;
	for( ssize_t i = 0; i < s; i++, t++ ) {
		const char *fmt = isprint(*t) || isspace(*t) ? "%c" : "0x%02x";
		fprintf(stderr, fmt, *t);
	}
	fflush(stderr);
	return s;
}

int
main(int argc, char **argv)
{
	int primary, secondary, c;
	ssize_t s;
	struct winsize ws = { .ws_row = 24, .ws_col = 80 };

	(void) argc;
	if( openpty(&primary, &secondary, NULL, NULL, &ws) ) {
		err(EXIT_FAILURE, "openpty");
	}
	switch( fork() ) {
	case -1:
		err(1, "fork");
		break;
	case 0:
		if( close(primary) ) {
			err(EXIT_FAILURE, "close");
		}
		if( login_tty(secondary) ) {
			err(EXIT_FAILURE, "login_tty");
		}
		execvp(argv[1], argv + 1);
		err(EXIT_FAILURE, "execvp %s", argv[1]);
	}
	/* Parent */
	if( close(secondary) ) {
		err(EXIT_FAILURE, "close");
	}

	send_msg(primary, NULL);
	fcntl(primary, F_SETFL, O_NONBLOCK);
	while( (c = getchar()) != EOF ) {
		ws.ws_row = 30 + c;
		ws.ws_col = 80 + c;
		char b[2] = {0};
		b[0] = c;
		send_msg(primary, b);
		send_msg(primary, "\e[A");
	}
	int saved_flags = fcntl(primary, F_GETFL);
	fcntl(primary, F_SETFL, primary & ~O_NONBLOCK);
	while( send_msg(primary, NULL) > 0 )
		;
	fputc('\n', stderr);

	return 0;
}
