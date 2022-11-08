/* Demonstrate fork, passing data to child through pipe */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>  /* For bcopy, for FD_COPY */
#include <sys/select.h>
#include <unistd.h>

void xclose(int fd);
void xdup2(int a, int b);
void xwrite(int fd, void *v, size_t s);
void xread(int fd, void *v, size_t s);
void xpipe(int *p);


struct data {
	int x, y, z;
	float f, g;
};

int
foo(void *v)
{
	struct data *d = v;
	d->z += 1;
	d->f /= 2;
	d->g *= 2;
	return 0;
}


int
count(fd_set *fd, int max)
{
	int c = 0;
	for( ; max; max -= 1 ){
		if( FD_ISSET(max, fd) ){
			c += 1;
		}
	}
	return c;
}


static void
show(const struct data *v)
{
	printf("{ %d, %d, %d, %f, %g }\n", v->x, v->y, v->z, v->f, v->g);
}

int
doit(int (*f)(void *), void *in, size_t s, fd_set *fds, int *max)
{
	int rv;
	int p2[2];
	xpipe(p2);

	switch( fork() ){
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
	case 0:
		xclose(p2[0]);
		rv = f(in);
		xwrite(p2[1], in, s);
		xclose(p2[1]);
		exit(rv);
	default:
		xclose(p2[1]);
	}
	FD_SET(p2[0], fds);
	*max = ( p2[0] > *max ) ? p2[0] : *max;
	return p2[0];
}


static void
handle(int maxfd, fd_set *fds)
{
	struct data val;
	for( int fd = 0; fd < maxfd + 1; fd += 1 ){
		if( FD_ISSET(fd, fds) ){
			FD_CLR(fd, fds + 1);
			xread(fd, &val, sizeof val);
			xclose(fd);
			show(&val);
			if( val.y < 2 ){
				val.y += 1;
				doit(foo, &val, sizeof val, fds + 1, &maxfd);
			}
		}
	}
}


int
main(int argc, char **argv)
{
	struct data val = { 1, 2, 3, 5.0, 1.2 };
	struct data result;
	int len;
	int maxfd = 0;
	fd_set fds[2];

	FD_ZERO(fds);

	for( int i = 0; i < 4; i += 1 ){
		val.x = i;
		val.y = 0;
		doit(foo, &val, sizeof val, fds + 1, &maxfd);
	}

	while( count(fds + 1, maxfd) ){
		FD_COPY(fds + 1, fds);
		switch( select(maxfd + 1, fds, NULL, NULL, NULL) ){
		default:
			handle(maxfd, fds);
			break;
		case -1:
			perror("select");
		case 0:
			assert(false);
		}
	}


	return EXIT_SUCCESS;
}


void
xclose(int fd)
{
	if( close(fd) ){
		perror("close");
		exit(EXIT_FAILURE);
	}
}

void
xwrite(int fd, void *v, size_t s)
{
	if( write(fd, v, s) != s ){
		perror("read");
		exit(1);
	}
}

void
xread(int fd, void *v, size_t s)
{
	if( read(fd, v, s) != s ){
		perror("read");
		exit(1);
	}
}

void
xdup2(int a, int b)
{
	if( dup2(a,b) == -1 ){
		perror("dup2");
		exit(EXIT_FAILURE);
	}
}

void
xpipe(int *p)
{
	if( pipe(p) ){
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}
