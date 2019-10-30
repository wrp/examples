
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void die( const char *message );

int
main( int argc, char **argv )
{
	int serv_sock;
	char message[ BUFSIZ ];
	int str_len;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	unsigned clnt_addr_size;
	uint16_t port;

	port = argc > 1 ? strtol( argv[1], NULL, 10 ) : 7853;

	serv_sock = socket( PF_INET, SOCK_DGRAM, 0 );
	if( serv_sock == -1 )
		die("socket");

	memset( &serv_addr, 0, sizeof serv_addr );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	serv_addr.sin_port = htons(port);

	if( bind( serv_sock, (struct sockaddr*) &serv_addr, sizeof serv_addr ) == -1)
		die("bind");

	while( (str_len = recvfrom(serv_sock, message, BUFSIZ, 0,
			(struct sockaddr*)&clnt_addr, &clnt_addr_size)) != 0) {
		message[str_len] = 0;
		if (!strcmp(message, "q\n"))
			break;
		write(1, message, str_len);
	}

	return 0;
}

void
die( const char *message )
{
	perror(message);
	exit(1);
}
