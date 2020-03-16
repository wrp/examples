
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

	if( s == 0 ) {
		s = sizeof(void *);
	}
	initial_size = 4 * s;
	align = sysconf(_SC_PAGESIZE);
	if( align > 0 && (st = malloc( sizeof *st)) != NULL) {
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

int
stack_push(struct stack *s, void *v)
{
	memcpy(s->top, v, s->element_size);
	return stack_incr(s) != NULL;
}


int
stack_pop(struct stack *s, void *v)
{
	if(s->top > s->data) {
		s->top = (char *)s->top - s->element_size;
		memcpy(v, s->top, s->element_size);
		return 1;
	}
	return 0;
}


void *
stack_get(struct stack *s, int idx)
{
	void *k = NULL;
	if( idx >= 0 && idx <= stack_size(s)) {
		k = (char *)s->data + idx * s->element_size;
	} else if( idx < 0 && idx >= -stack_size(s)) {
		k = (char *)s->top + idx * s->element_size;
	}
	return k;
}
