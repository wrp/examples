#include <assert.h>
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

static void
show_data(const char *t, ssize_t s)
{
	for( ; s > 0; t++, s-- ) {
		const char *fmt = isprint(*t) || isspace(*t) ? "%c" : "0x%02x";
		fprintf(stderr, fmt, *t);
	}
	fflush(stderr);
}

static ssize_t
send_msg(int fd, int b)
{
	char buf[1024];
	if( b != EOF ) {
		write(fd, &b, 1);
	}
	ssize_t s = read(fd, buf, sizeof buf);
	show_data(buf, s);
	return s;
}

static void
wait_for(int fd, const char *expect, size_t siz)
{
	int rc = 0;
	char buf[1024];
	char *a = buf;
	assert(siz < sizeof buf);
	char *end = buf + siz;

	while( a < end ) {
		ssize_t s = read(fd, a, end - a);
		char *e = a + s;
		rc += 1;
		while( a < e && *a++ == *expect++ ) {
			;
		}
		if( s < 1 || a < e ) {
			fprintf(stderr, "Ivalid data\nReceived: ");
			show_data(buf, e - buf);
			fprintf(stderr, "Expected: ");
			show_data(expect, siz);
			exit(1);
		}
	}
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

	/* Initialization sequence from pty on macos */
	char *expected = "\x1b(B\x1b)0\x1b[?1049h\x1b[1;24r\x1b[m\x0f\x1b[4l"
		"\x1b[?1h\x1b=\x1b[H\x1b[J";
	wait_for(primary, expected, strlen(expected));
	fcntl(primary, F_SETFL, O_NONBLOCK);
	while( (c = getchar()) != EOF ) {
		send_msg(primary, c);
	}
	int saved_flags = fcntl(primary, F_GETFL);
	fcntl(primary, F_SETFL, primary & ~O_NONBLOCK);
	while( send_msg(primary, EOF) > 0 )
		;
	fputc('\n', stderr);

	return 0;
}
