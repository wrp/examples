/* Simple server to read from a local socket and write to stdout */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

void die(const char *message);
void rewrite(int fd, const char *b, size_t n);

int
main(int argc, char **argv)
{
	int serv_sock;
	char message[BUFSIZ] = "";
	int cfd;
	struct sockaddr_un serv_addr;
	struct sockaddr_un clnt_addr;
	struct sockaddr *sa = (void *)&serv_addr;
	struct sockaddr *ca = (void *)&clnt_addr;
	unsigned clnt_addr_size;
	const char *path = argc > 1 ? argv[1] : "foo";

	memset(&serv_addr, 0, sizeof serv_addr);
	serv_addr.sun_family = AF_UNIX;
	strncpy(serv_addr.sun_path, path, sizeof(serv_addr.sun_path) - 1);

	serv_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if( serv_sock == -1 ){
		die("socket");
	}
	int o = 1;

	if( bind(serv_sock, sa, sizeof serv_addr) == -1 ){
		die(path);
	}

	if( listen(serv_sock, 5) ){
		die("listen");
	}

accept_loop:
	if( (cfd = accept(serv_sock, ca, &clnt_addr_size)) < 0 ){
		die("accept");
	}

	printf("connected\n");

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
	printf("connection closed\n");
	if( strncmp(message, "q\n", 2) ){
		goto accept_loop;
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
