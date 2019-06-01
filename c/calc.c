/*
 * A simple reverse polish calculator
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

struct state {
	double *stack, *sp;
	size_t stack_size;
	char buf[32], *bp;
	int width;
	int precision;
};


void
apply_operator(struct state *S, int c)
{
	char *end;
	char fmt[32];
	*S->bp = '\0';
	if( S->bp != S->buf ) {
		*(++S->sp) = strtod(S->buf, &end);
		if( end != S->bp ) {
			fprintf(stderr, "Garbled: %s\n", S->buf);
		}
		S->bp = S->buf;
	}

	switch(c) {
	case '*': S->sp -= 1; S->sp[0] *= S->sp[1]; break;
	case '+': S->sp -= 1; S->sp[0] += S->sp[1]; break;
	case '/': S->sp -= 1; S->sp[0] /= S->sp[1]; break;
	case '^': S->sp -= 1; S->sp[0] = pow(S->sp[0], S->sp[1]); break;
	case '-': S->sp -= 1; S->sp[0] -= S->sp[1]; break;
	case 'k': S->precision = *S->sp--; break;
	case 'w': S->width = *S->sp--; break;
	case 'p':
		snprintf(fmt, sizeof fmt, "%%%d.%dg\n", S->width, S->precision);
		printf(fmt, S->sp[0]);
		break;
	case 'q': exit(1);
	}
}


void
realloc_stack( struct state *S )
{
	assert( S->sp == NULL || S->sp - S->stack == S->stack_size - 1);
	if( ( S->stack = realloc(S->stack, S->stack_size * 2 )) == NULL) {
		perror("realloc");
		exit(1);
	}
	S->sp = S->stack + S->stack_size - 1;
	S->stack_size *= 2;
}


int
main(int argc, char **argv)
{
	int c;
	struct state S[1];

	S->stack = NULL;
	S->stack_size = 1;
	S->sp = S->stack;
	realloc_stack(S);
	S->bp = S->buf;
	S->width = 6;
	S->precision = 3;

	while( (c=getchar()) != EOF ) {
		if(strchr("*+/^-kwpq \t\n", c)) {
			apply_operator(S,c);
		} else {
			*S->bp++ = (char)c;
		}
		if( S->sp == S->stack ) {
			fprintf(stderr, "Stack empty\n");
			S->sp = S->stack + 1;
		} else if ( S->sp - S->stack == S->stack_size - 1) {
			realloc_stack(S);
		}
	}
	return 0;
}
