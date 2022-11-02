/* Demonstrate fork, passing data to child through pipe */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void xclose(int fd);
void xdup2(int a, int b);
void xwrite(int fd, void *v, size_t s);
void xread(int fd, void *v, size_t s);


struct data {
	int x, y, z;
	float f, g;
};

int
foo(void *v)
{
	struct data *d = v;
	d->x += 1;
	d->y += 1;
	d->z += 1;
	d->f /= 2;
	d->g *= 2;
	return 0;
}


static void
show(const struct data *v)
{
	printf("{ %d, %d, %d, %f, %g }\n", v->x, v->y, v->z, v->f, v->g);
}


int
doit(int (*f)(void *), size_t s, int fdi, int fdo)
{
	unsigned char d[s];
	int rv;

	xread(fdi, d, s);
	rv = foo(d);
	xwrite(fdo, d, s);

	return rv;
}

int
main(int argc, char **argv)
{
	int p1[2];
	int p2[2];
	struct data val = { 1, 2, 3, 5.0, 1.2 };
	struct data result;
	int len;
	if( pipe(p1) || pipe(p2) ){
		perror("pipe");
		return EXIT_FAILURE;
	}
	switch( fork() ){
	case -1:
		perror("fork");
		return EXIT_FAILURE;
	case 0:
		xclose(p1[1]);
		xclose(p2[0]);
		exit(doit(foo, sizeof val, p1[0], p2[1]));
	default:
		xwrite(p1[1], &val, sizeof val);
		xclose(p1[1]);
		xclose(p1[0]);
		xclose(p2[1]);
		xread(p2[0], &result, sizeof result);
		xclose(p2[0]);
		show(&val);
		show(&result);
	}
	if( fclose(stdout) ){
		perror("stdout");
		return EXIT_FAILURE;
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
