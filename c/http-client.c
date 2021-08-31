
#include <arpa/inet.h>
#include <err.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int sock;
	int count;
	struct sockaddr_in serv_addr;
	struct sockaddr *sa = (void*)&serv_addr;
	char *hostname = argc > 1 ? argv[1] : "www.google.com";
	int port = argc > 2 ? strtoul(argv[2], NULL, 10) : 80;
	char buffer[1024];
	char request[1024];

	snprintf(request,
		sizeof request,
		"GET / HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Connection: close\r\n\r\n",
		hostname
	);

	struct hostent *hostinfo = gethostbyname(hostname);
	char *ip = inet_ntoa(*(struct in_addr*)hostinfo->h_addr_list[0]);

	if( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
		err(EXIT_FAILURE, "Socket");
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	switch( inet_pton(AF_INET, ip, &serv_addr.sin_addr) ) {
	case -1: err(EXIT_FAILURE, "inet_pton");
	case 0: errx(EXIT_FAILURE, "address not parseable");
	}

	printf("Connecting to %s...\n", ip);

	if( connect(sock, sa, sizeof *sa) < 0 ){
		err(EXIT_FAILURE, "connect");
	}
	send(sock, request, strlen(request), 0);

	/* See also: use recvfrom */
	while( (count = read(sock, buffer, sizeof buffer)) > 0 ){
		fwrite(buffer, 1, count, stdout);
	}
	putchar('\n');
	return 0;
}
