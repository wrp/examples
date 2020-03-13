/* A simple ring buffer */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct ring_buf {
	int err;
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
		} /* uncovered */
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
		return 1; /* uncovered */
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
rb_clear(struct ring_buf *r)
{
	r->start = r->end = r->buf;
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
rb_tail( struct ring_buf *R )
{
	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( R->start == R->end ) {
		return EOF;
	}
	if(R->end == R->buf) {
		R->end == R->buf + R->s;
	}
	return (int)*--R->end;
}


int
rb_string(struct ring_buf *r, char *dest, size_t siz)
{
	unsigned char *s = r->start;
	unsigned char *end = r->start <= r->end ? r->end : r->buf + r->s;

	siz -= 1;  /* leave space for terminator */
	for( ; *s && s < end && siz--; s++ ) {
		*dest++ = *s;
	}
	if( r->start > r->end ) {
		for( s=r->buf; *s && s < r->end && siz--; s++ ) {
			*dest++ = *s;
		}
	}
	*dest = '\0';
	return s == r->end;
}


int
rb_length(struct ring_buf *r)
{
	if(r->end > r->start)
		return r->end - r->start;
	else
		return r->s - (r->start - r->end);
}
