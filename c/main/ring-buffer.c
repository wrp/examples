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
	unsigned char *end;   /* One past final byte of data */
	size_t s;             /* Total allocated size */
};

/*
 * Create an initialized a ring buffer.
 */
struct ring_buf *
rb_create(size_t s)
{
	struct ring_buf *r = malloc(sizeof *r);
	if( r != NULL ){
		r->s = s ? s : 1024;
		r->end = r->start = r->buf = malloc(r->s);
		r->err = 0;
		if( r->buf == NULL ){
			free(r); /* uncovered */
			r = NULL; /* uncovered */
		} /* uncovered */
	}
	return r;
}

void
rb_free(struct ring_buf *r)
{
	if( r ){
		free(r->buf);
		free(r);
	}
}

static int
grow(struct ring_buf *R)
{
	unsigned char *tmp = realloc(R->buf, 2 * R->s);
	assert( R->end == R->start );
	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( tmp == NULL ){
		return 1; /* uncovered */
	}
	if( R->buf != tmp ){
		R->end = R->start = tmp + (R->start - R->buf);
		R->buf = tmp;
	}
	if( R->end != R->buf ){
		memmove(R->buf + R->s, R->buf, R->end - R->buf);
	}
	R->end = R->start + R->s;
	R->s *= 2;
	return 0;
}

int
rb_push(struct ring_buf *R, unsigned char c)
{
	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( R->err == 0 ){
		*R->end++ = c;
		if( R->end == R->buf + R->s ){
			R->end = R->buf;
		}
		if( R->end == R->start ){
			R->err = grow(R);
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
rb_pop(struct ring_buf *R)
{
	unsigned char rv;
	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( R->start == R->end ){
		return EOF;
	}
	rv = (int)*R->start++;
	if( R->start == R->buf + R->s ){
		R->start = R->buf;
	}
	return rv;
}


int
rb_peek(struct ring_buf const *R, size_t idx)
{
	unsigned char *rv = NULL;

	assert( R->start >= R->buf );
	assert( R->start < R->buf + R->s );
	assert( R->end >= R->buf );
	assert( R->end < R->buf + R->s );
	if( R->start == R->end ){
		;
	} else if( R->end > R->start ){
		if( R->start + idx < R->end ){
			rv = R->start + idx;
		}
	} else {
		if( R->start + idx < R->buf + R->s ){
			rv = R->start + idx;
		} else if( R->start + idx < R->end + R->s ){
			rv = R->start + idx - R->s;
		}
	}
	return rv ? *rv : EOF;
}
