#include "stack.h"
#include <string.h>



struct stack {
	void *data; /* The base (allocated) */
	void *top;  /* One beyond top element */
	void *end;  /* One behond last allocated */
	size_t element_size;
};

struct stack *
stack_create(size_t el)
{
	struct stack *st;
	int initial_size = 4 * el;

	if( ! posix_memalign((void *)&st, sizeof(void *), sizeof *st)) {
		st->element_size = el;
		if (! posix_memalign((void *)&st->data, sizeof(void *), initial_size)) {
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
