/* Simple server to read from a socket and write to stdout */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void die(const char *message);
void rewrite(int fd, const char *b, size_t n);

int
main(int argc, char **argv)
{
	int serv_sock;
	char message[BUFSIZ] = "";
	struct hostent *host;
	int cfd;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	struct sockaddr *sa = (void *)&serv_addr;
	struct sockaddr *ca = (void *)&clnt_addr;
	unsigned clnt_addr_size;
	uint16_t port;

	port = argc > 1 ? strtol(argv[1], NULL, 10) : 7853;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if( serv_sock == -1 ){
		die("socket"); /* uncovered */
	}
	int o = 1;
	if( setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &o, sizeof o) ){
		die("setsockopt");
	}

	memset(&serv_addr, 0, sizeof serv_addr);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if( bind(serv_sock, sa, sizeof serv_addr)== -1 ){
		die("bind"); /* uncovered */
	}

	if( listen(serv_sock, 5) ){
		die("listen"); /* uncovered */
	}

accept_loop:
	if( (cfd = accept(serv_sock, ca, &clnt_addr_size)) < 0 ){
		die("accept"); /* uncovered */
	}

	host = gethostbyaddr(&clnt_addr.sin_addr.s_addr,
		sizeof clnt_addr.sin_addr.s_addr, clnt_addr.sin_family);

	char *hostaddr = inet_ntoa(clnt_addr.sin_addr);
	if( hostaddr == NULL ){
		die("inet_ntoa");
	}
	printf("connected to %s (%s)\n", host ? host->h_name : "?", hostaddr);

	ssize_t s;
	while( (s = read(cfd, message, sizeof message)) != 0 ){;
		switch( s ){
		case -1: die("read");
		default: rewrite(STDOUT_FILENO, message, s);
		}
	}
	if( close(cfd) ){
		die("close");
	}
	printf("connection closed (%s)\n", hostaddr);
	if( strncmp(message, "q\n", 2) ){
		goto accept_loop;
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
