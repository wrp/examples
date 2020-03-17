/* A simple ring buffer */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct ring_buf {
	int err;
	unsigned char *buf;   /* Allocated base */
	unsigned char *start; /* First byte of data */
	unsigned char *end;   /* One last final byte of data */
	size_t s;             /* Total allocated size */
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
		r->err = 0;
		if( r->buf == NULL ) {
			free( r ); /* uncovered */
			r = NULL;  /* uncovered */
		} /* uncovered */
	}
	return r;
}


void
rb_free(struct ring_buf *r)
{
	if(r) {
		free(r->buf);
		free(r);
	}
}


static int
grow( struct ring_buf *R )
{
	unsigned char *tmp = realloc( R->buf, 2 * R->s );
	assert( R->end == R->start );
	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( tmp == NULL ) {
		return 1; /* uncovered */
	}
	if( R->buf != tmp ) {
		R->end = R->start = tmp + ( R->start - R->buf );
		R->buf = tmp;
	}
	if(R->end != R->buf) {
		memmove( R->buf + R->s, R->buf, R->end - R->buf );
	}
	R->end = R->start + R->s;
	R->s *= 2;
	return 0;
}


int
rb_push( struct ring_buf *R, unsigned char c )
{
	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( R->err == 0 ) {
		*R->end++ = c;
		if( R->end == R->buf + R->s ) {
			R->end = R->buf;
		}
		if( R->end == R->start ) {
			R->err = grow( R );
		}
	}
	assert( R->end != R->start || R->err == 1 );
	return R->err;
}


int
rb_isempty(struct ring_buf *r)
{
	return r->start == r->end;
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


int
rb_peek(struct ring_buf const *R, size_t idx)
{
	unsigned char *retv = NULL;

	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( R->start == R->end ) {
		;
	} else if( R->end > R->start ) {
		if( idx < R->end - R->start ) {
			retv = R->start + idx;
		}
	} else {
		ptrdiff_t off = R->buf + R->s - R->start;
		if( idx < off ) {
			retv = R->start + idx;
		} else if( idx - off < R->end - R->buf ) {
			retv = R->buf + ( idx - off );
		}
	}
	return retv ? *retv : EOF;
}
