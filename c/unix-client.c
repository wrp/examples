/* Simple client to read stdin and write to a unix socket */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

void die(const char *message);
void rewrite(int fd, const char *b, size_t n);

int
main(int argc, char **argv)
{
	int sock;
	char message[BUFSIZ];
	struct sockaddr_un serv_addr;
	struct sockaddr *sa = (void*)&serv_addr;
	const char *path = argc > 1 ? argv[1] : "foo";

	memset(&serv_addr, 0, sizeof serv_addr);
	serv_addr.sun_family = AF_UNIX;

	if( (sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		die("socket");
	}

	strncpy(serv_addr.sun_path, path, sizeof(serv_addr.sun_path) - 1);

	if( connect(sock, sa, sizeof serv_addr ) < 0 ){
		die("connect");
	}
	while( fgets(message, BUFSIZ, stdin) != NULL) {
		rewrite(sock, message, strlen(message));
		if( !strcmp(message, "q\n") ){
			break;
		}
	}
	if( close(sock) ){
		die("close");
	}
	return 0;
}

void
die(const char *message)
{
	perror(message);
	exit(1);
}

void
rewrite(int fd, const char *b, size_t n)
{
        const char *e = b + n;
        ssize_t s;
        if( n > 0 ) do {
                s = write(fd, b, e - b);
		if( s < 0 && errno != EINTR ){
			die("write");
		}
                b += s < 0 ? 0 : s;
        } while( b < e );
}
