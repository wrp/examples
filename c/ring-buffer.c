/* A simple ring buffer */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct ring_buf {
	unsigned char *buf;
	unsigned char *start;
	unsigned char *end;
	size_t s;
};

/*
 * Create an initialized a ring buffer.
 */
struct ring_buf *
rb_create( size_t s )
{
	struct ring_buf *r;
	r = malloc( sizeof *r );
	if( r != NULL ) {
		r->end = r->start = r->buf = malloc( r->s = s ? s : 1024 );
		if( r->buf == NULL ) {
			free( r ); /* uncovered */
			r = NULL;  /* uncovered */
		}
	}
	return r;
}

static int
grow( struct ring_buf *R )
{
	unsigned char *tmp = realloc( R->buf, 2 * R->s );
	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( tmp == NULL ) {
		return 0; /* uncovered */
	}
	if( R->buf != tmp ) { /* uncovered block */
		R->start = tmp + ( R->start - R->buf );
		R->end = tmp + ( R->end - R->buf );
		R->buf = tmp;
	} /* uncovered end */
	if( R->end <= R->start ) {
		memmove( R->buf + R->s, R->buf, R->end - R->buf );
		R->end = R->end + R->s;
	}
	R->s *= 2;
	return 1;
}


int
rb_push( struct ring_buf *R, unsigned char c )
{
	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	*R->end++ = c;
	if( R->end == R->buf + R->s ) {
		R->end = R->buf;
	}
	if( R->end == R->start ) {
		if( ! grow( R )) {
			return 0;
		}
	}
	assert( R->end != R->start );
	return 1;
}

int
rb_pop( struct ring_buf *R )
{
	unsigned char rv;
	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( R->start == R->end ) {
		return EOF;
	}
	rv = (int)*R->start++;
	if( R->start == R->buf + R->s ) {
		R->start = R->buf;
	}
	return rv;
}
