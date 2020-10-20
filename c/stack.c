
#include "stack.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/* Keep INITIAL_SIZE low to trigger reallocs for coverage test. */
#define INITIAL_SIZE 4

struct stack {
	void *data; /* The base (allocated) */
	void *top;  /* One beyond top element */
	void *end;  /* One behond last allocated */
	size_t element_size;
	unsigned flags;
	size_t align;
	int raw;
};

struct stack *
stack_xcreate(size_t s)
{
	struct stack *rv = stack_create(s);
	if( rv == NULL ) {
		perror("stack_create");
		exit(EXIT_FAILURE);
	}
	return rv;
}

/* Note that using memalign is pretty pointless since we
 * are going to realloc, which may discard the alignment.
 */
static void *
alloc(size_t alignment, size_t s)
{
	void *rv = alignment ? NULL : malloc(s);
	if( rv == NULL && posix_memalign(&rv, alignment, s) ) {
		rv = NULL;
	}
	return rv;
}

struct stack *
stack_create(size_t s)
{
	int initial_size;
	struct stack *st = NULL;

	if( (st = malloc(sizeof *st)) != NULL ) {
		long align = sysconf(_SC_PAGESIZE);
		st->align = align == -1 ? 0 : align;
		st->raw = !s;
		st->element_size = s ? s : sizeof st->data;
		initial_size = INITIAL_SIZE * st->element_size;

		st->top = st->data = alloc(st->align, initial_size);
		if( st->data == NULL ) {
			int e = errno;
			free(st);
			errno = e;
			st = NULL;
		} else {
			st->end = (char *)st->data + initial_size;
		}
	}
	return st;
}

void *
stack_base(struct stack *s)
{
	return s->data;
}

static void *
stack_incr(struct stack *s)
{
	char *t = s->top;
	t += s->element_size;
	s->top = t;
	void *tmp = s->top;
	if( s->top == s->end ) {
		ptrdiff_t o = t - (char *)s->data;
		void *tmp = realloc(s->data, o + INITIAL_SIZE);
		if( tmp != NULL ) {
			s->data = tmp;
			s->top = s->data + o;
			s->end = s->data + o + INITIAL_SIZE;
		}
	}
	return tmp;
}

size_t
stack_size(struct stack *s)
{
	return (s->top - s->data) / s->element_size;
}


void
stack_xpush(struct stack *s, void *v)
{
	if(! stack_push(s, v)) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
}

int
stack_push(struct stack *s, void *v)
{
	if( s->raw ) {
		*(void **)s->top = v;
	} else {
		memcpy(s->top, v, s->element_size);
	}
	return stack_incr(s) != NULL;
}


int
stack_pop(struct stack *s, void *v)
{
	int rv = s->top > s->data;
	if( rv ) {
		s->top = (char *)s->top - s->element_size;
		if( s->raw ) {
			*(void **)v = *(void **)s->top;
		} else {
			memcpy(v, s->top, s->element_size);
		}
	}
	return rv;
}


void *
stack_get(struct stack *s, int idx)
{
	void *v = NULL;
	char *base = NULL;
	if( idx >= 0 && (unsigned)idx <= stack_size(s)) {
		base = s->data;
	} else if( idx < 0 && (unsigned)-idx <= stack_size(s)) {
		base = s->top;
	}
	if( base != NULL ) {
		if( s->raw ) {
			v = *(void **)(base + idx * sizeof v);
		} else {
			v = base + idx * s->element_size;
		}
	}
	return v;
}
