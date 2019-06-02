/*
 * A simple reverse polish calculator
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define operators "*+/^-kpq"

struct state {
	double *stack, *sp;
	size_t stack_size;
	char buf[32], *bp;
	int precision;
};

void process_entry( struct state *S, int c );
void realloc_stack( struct state *S );
void apply_operator(struct state *S, int c);
void realloc_stack( struct state *S );

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
	S->precision = 3;

	if( argc > 1) {
		for( argv += 1; *argv; argv++ ) {
			for( char *k = *argv; *k; k++ ) {
				process_entry(S, (int)*k);
			}
			process_entry(S, ' ');
		}
	} else while( (c=getchar()) != EOF ) {
		process_entry(S, c);
	}
	return 0;
}

void
process_entry(struct state *S, int c)
{
	if(strchr( operators " \t\n", c)) {
		apply_operator(S,c);
	} else {
		*S->bp++ = (char)c;
	}
}

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
	case 'p':
		snprintf(fmt, sizeof fmt, "%%.%dg\n", S->precision);
		printf(fmt, S->sp[0]);
		break;
	case 'q': exit(1);
	/* Attempt to assure consistency with this case statement. */
	default: assert(!strchr(operators, c));
	}
	if( S->sp == S->stack ) {
		fprintf(stderr, "Stack empty\n");
		S->sp = S->stack + 1;
	} else if ( S->sp - S->stack == S->stack_size - 1) {
		realloc_stack(S);
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
