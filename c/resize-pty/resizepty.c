/* Trivial example that spawns a program in a pty and resizes the pty */
#include <ctype.h>
#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>
#ifdef __linux__
# include <pty.h>
# include <utmp.h>
#else
# include <util.h>
#endif

static void grep(int fd, const char *needle);

int
main(void)
{
	int status;
	int fd[2];
	ssize_t s;

	if( openpty(fd, fd + 1, NULL, NULL, NULL) ) {
		err(EXIT_FAILURE, "openpty");
	}
	switch( fork() ) {
	case -1:
		err(1, "fork");
		break;
	case 0:
		if( close(fd[0]) ) {
			err(EXIT_FAILURE, "close");
		}
		if( login_tty(fd[1]) ) {
			err(EXIT_FAILURE, "login_tty");
		}
		{
		char *argv[] = { "./show-size", NULL };
		execv(argv[0], argv);
		err(EXIT_FAILURE, "execv %s", argv[0]);
		}
	}

	char buf[1024];
	int c;
	while( (c = getchar()) != EOF ) {
		fprintf(stderr, "Read %02x\n", c);
		struct winsize ws = { .ws_row = 60 + c, .ws_col = 80 + c};
		if( ioctl(fd[0], TIOCSWINSZ, &ws) ) {
			err(EXIT_FAILURE, "ioctl");
		}

		s = read(fd[0], buf, sizeof buf);
		if( s < 1 ) {
			err(1, "Read");
		}
		int nonprint = 0;
		buf[s] = '\0';
		for( ssize_t i = 0; i < s; i++ ) {
			const char t = buf[i];
			if( t == 0x1b ) {
				nonprint = 1;
			} else if( !nonprint ) {
				fputc(t, stderr);
			} else if( t == 'H' || t == 'J' ){
				nonprint = 0;
			}
		}
	}

	wait(&status);
	return status;
}

/* For synchronization.  Block until needle is read from fd */
static void
grep(int fd, const char *needle)
{
	char buf[BUFSIZ];
	const char *end = buf;
	const char *b = buf;
	const char *n = needle;
	while( *n != '\0' ) {
		if( b == end ) {
			ptrdiff_t d = n - needle;
			if( d > 0 ) {
				memcpy(buf, b - d, d);
			}
			size_t rc = read(fd, buf + d, sizeof buf - d);
			switch( rc ) {
			case -1: err(EXIT_FAILURE, "read from pty");
			case 0: return;
			}
			end = buf + d + rc;
			b = buf + d;
		}
		if( *b++ != *n++ ) {
			n = needle;
		}
	}
}
