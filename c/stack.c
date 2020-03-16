
#include "stack.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct stack {
	void *data; /* The base (allocated) */
	void *top;  /* One beyond top element */
	void *end;  /* One behond last allocated */
	size_t element_size;
	unsigned flags;
	int raw;
};

struct stack *
stack_xcreate(size_t s) {
	struct stack *rv;
	if( (rv = stack_create(s)) == NULL) {
		perror("stack_create");
		exit(EXIT_FAILURE);
	}
	return rv;
}

struct stack *
stack_create(size_t s)
{
	int initial_size;
	long align;
	struct stack *st = NULL;

	align = sysconf(_SC_PAGESIZE);
	if( align > 0 && (st = malloc( sizeof *st)) != NULL) {
		st->raw = 0;
		if( s == 0 ) {
			st->raw = 1;
			s = sizeof(void *);
		}
		initial_size = 4 * s;
		st->element_size = s;
		if (! posix_memalign((void *)&st->data, align, initial_size)) {
			st->top = st->data;
			st->end = (char *)st->data + initial_size;
		} else {
			free(st);
		}
	}
	return st;
}

void *
stack_base(struct stack *s)
{
	return s->data;
}

void *
stack_incr(struct stack *s)
{
	char *t = s->top;
	t += s->element_size;
	s->top = t;
	if(s->top == s->end) {
		ptrdiff_t o = t - (char *)s->data;
		s->end = s->top = s->data = realloc(s->data, 2 * o);
		if(s->data != NULL) {
			s->top = s->data + o;
			s->end = s->data + 2 * o;
		}
	}
	return s->top;
}

void *
stack_decr(struct stack *s)
{
	char *t = s->top;
	if( s->top > s->data) {
		t -= s->element_size;
		s->top = t;
	}
	return s->top;
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


void *
stack_pop(struct stack *s, void *v)
{
	void *rv = NULL;
	if(s->top > s->data) {
		s->top = (char *)s->top - s->element_size;
		if( s->raw ) {
			rv = *(void **)s->top;
			*(void**) v = s->top;
		} else {
			rv = v;
			memcpy(v, s->top, s->element_size);
		}
	} else if( s->raw ){
		*(void**) v = NULL;
	}
	return rv;
}


void *
stack_get(struct stack *s, int idx)
{
	void *v = NULL;
	char *base = NULL;
	if( idx >= 0 && idx <= stack_size(s)) {
		base = s->data;
	} else if( idx < 0 && -idx <= stack_size(s)) {
		base = s->top;
	}
	if(base != NULL) {
		if( s->raw ) {
			v = *(void **)(base + idx * (sizeof v));
		} else {
			v = base + idx * s->element_size;
		}
	}
	return v;
}
