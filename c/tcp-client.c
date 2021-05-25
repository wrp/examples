/* Simple client to read stdin and write to a tcp socket */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void die(const char *message);
void rewrite(int fd, const char *b, size_t n);

int
main(int argc, char **argv)
{
	int sock;
	char message[BUFSIZ];
	char *host = argc > 1 ? argv[1] : "127.0.0.1";
	uint16_t port = argc > 2 ? strtol( argv[2], NULL, 10 ) : 7853;
	struct sockaddr_in serv_addr;
	struct sockaddr *sa = (void*)&serv_addr;
	struct hostent *server;

	if( (sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		die("socket"); /* uncovered */
	}

	if( (server = gethostbyname(host)) == NULL ){
		die("gethostbyname"); /* uncovered */
	}

	memset(&serv_addr, 0, sizeof serv_addr);
	serv_addr.sin_family = AF_INET;
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	/* serv_addr.sin_addr.s_addr = inet_addr(host); */
	serv_addr.sin_port = htons(port);

	if( connect(sock, sa, sizeof serv_addr ) < 0 ){
		die("connect"); /* uncovered */
	}
	while( fgets(message, BUFSIZ, stdin) != NULL) {
		rewrite(sock, message, strlen(message));
		if( !strcmp(message, "q\n") ){
			break;
		}
	}
	if( close(sock) ){
		die("close"); /* uncovered */
	}
	return 0;
}

void
die(const char *message) /* uncovered */
{
	perror(message); /* uncovered */
	exit(1); /* uncovered */
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
