/* Simple client to read stdin and write to a udp socket */

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
	int sock;
	char message[BUFSIZ];
	char *host = argc > 1 ? argv[1] : "127.0.0.1";
	uint16_t port = argc > 2 ? strtol( argv[2], NULL, 10 ) : 7853;
	struct sockaddr_in serv_addr;

	if( (sock = socket( PF_INET, SOCK_DGRAM, 0 )) == -1 )
		die("socket"); /* uncovered */

	memset( &serv_addr, 0, sizeof serv_addr );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(host);
	serv_addr.sin_port = htons(port);

	while( fgets( message, BUFSIZ, stdin ) != NULL ) {

		if( -1 == sendto(
				sock,
				message,
				strlen( message ),
				0,
				(struct sockaddr*)&serv_addr,
				sizeof serv_addr
			)
		) {
			die("sendto"); /* uncovered */
		}
		if( !strcmp( message, "q\n" )) {
			break;
		}
	}
	if( close(sock))
		die("close"); /* uncovered */
	return 0;
}

void
die(const char *message) /* uncovered */
{
	perror(message); /* uncovered */
	exit(1); /* uncovered */
}
