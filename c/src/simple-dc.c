/* A simple post-fix calculator. */
#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct stack {
	double *data;
	size_t len;
	size_t cap;
};

void
push(double v, struct stack *s)
{
	if (s->len >= s->cap) {
		s->data = realloc(s->data, s->cap += 128);
		if (s->data == NULL) {
			err(1, "realloc");
		}
	}
	s->data[s->len++] = v;
}

double
pop(struct stack *s)
{
	if (s->len == 0) {
		errx(1, "underflow");
	}
	return s->data[--s->len];
}

int
main(int argc, char **argv)
{
	struct stack s = {0};
	int c;
	while ( (c = getchar()) != EOF ){
		double a, b;
		if (isdigit(c)) {
			double accum = 0.0;
			do {
				accum = 10 * accum + c - '0';
			} while (isdigit(c = getchar()));
			push(accum, &s);
		}
		switch(c) {
		case '+': push(pop(&s) + pop(&s), &s); break;
		case '*': push(pop(&s) * pop(&s), &s); break;
		case '-': a = pop(&s); b = pop(&s); push(b - a, &s); break;
		case '/': a = pop(&s); b = pop(&s); push(b / a, &s); break;
		case 'p': printf("%g\n", pop(&s)); break;
		case ' ': case '\n': case ',': break;
		case EOF: return 0;
		default: errx(1,  "invalid entry: %c\n", c);
		}
	}
	return 0;
}
