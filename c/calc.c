/*
 * A simple reverse polish calculator
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

#define operators "*+/^-"
#define commands "dfkpq"

struct state {
	long double *stack, *sp;
	size_t stack_size;
	char *buf, *bp;
	size_t buf_size;
	char fmt[32];
};

void process_entry( struct state *S, int c );
void apply_operator( struct state *S, int c );
void apply_command( struct state *S, int c );
void grow_stack( struct state *S );
void * xrealloc( void *p, size_t s );
void die( const char *msg );
void write_args_to_stdin( char *const*argv );
void push_number( struct state *S );
int
main( int argc, char **argv )
{
	int c;
	struct state S[1];

	S->stack_size = 4;
	S->stack = xrealloc( NULL, sizeof *S->stack * S->stack_size );
	S->sp = S->stack;
	S->buf_size = 1;
	S->buf = xrealloc( NULL, sizeof *S->buf * S->buf_size );
	S->bp = S->buf;
	strcpy( S->fmt, "%.3Lg\n" );

	if( argc > 1) {
		for( ; *++argv; process_entry( S, ' ')) {
			for( char *t = *argv; *t; t++ ) {
				process_entry( S, *t );
			}
		}
	} else while( (c=getchar()) != EOF ) {
		process_entry( S, c );
	}
	return 0;
}


void push_buf(struct state *S, int c)
{
	*S->bp++ = (char)c;
	if( S->bp == S->buf + S->buf_size ) {
		S->buf = xrealloc( S->buf, S->buf_size * 2 * sizeof *S->buf );
		S->bp = S->buf + S->buf_size;
		S->buf_size *= 2;
	}
}

void
process_entry(struct state *S, int c)
{
	if( strchr( " \t\n", c )) {
		push_number(S);
	} else if(strchr( operators, c )) {
		push_number(S);
		apply_operator(S, c);
	} else if(strchr( commands, c )) {
		push_number(S);
		apply_command(S, c);
	} else {
		push_buf(S, c);
	}
}


void
push_number(struct state *S)
{
	*S->bp = '\0';
	if( S->bp != S->buf ) {
		char *end;
		*(S->sp++) = strtold(S->buf, &end);
		if( end != S->bp ) {
			fprintf(stderr, "Garbled: %s\n", S->buf);
		}
		S->bp = S->buf;
		if( S->sp == S->stack + S->stack_size ) {
			grow_stack(S);
		}
	}
}


void
apply_command(struct state *S, int c)
{
	assert( S->sp >= S->stack );
	assert( strchr( commands, c ));
	if( S->sp - S->stack < 1 ) {
		fputs( "Stack empty (need 1 value)\n", stderr );
		return;
	}
	switch(c) {
	case 'd':
		S->sp[0] = S->sp[-1];
		S->sp += 1;
		break;
	case 'k':
		snprintf(S->fmt, sizeof S->fmt, "%%.%dLg\n", (int)*--S->sp);
		break;
	case 'f':
		for(typeof(S->stack) s = S->stack; s < S->sp; s++) {
			printf("%3u: ", (unsigned)(s - S->stack));
			printf(S->fmt, *s);
		}
		break;
	case 'p': {
		printf(S->fmt, S->sp[-1]);
	} break;
	case 'q': exit(0);
	}
}


void
apply_operator(struct state *S, int c)
{
	assert( S->sp >= S->stack );
	if( S->sp - S->stack < 2 ) {
		fputs( "Stack empty (need 2 values)\n", stderr );
		return;
	}
	S->sp -= 1;
	switch(c) {
	case '*': S->sp[-1] *= S->sp[0]; break;
	case '+': S->sp[-1] += S->sp[0]; break;
	case '/': S->sp[-1] /= S->sp[0]; break;
	case '-': S->sp[-1] -= S->sp[0]; break;
	case '^': S->sp[-1] = pow(S->sp[-1], S->sp[0]); break;
	}
}


void
grow_stack( struct state *S )
{
	assert( S->sp == S->stack + S->stack_size );
	S->stack = xrealloc(S->stack, S->stack_size * 2 * sizeof *S->stack );
	S->sp = S->stack + S->stack_size;
	S->stack_size *= 2;
}


void *
xrealloc( void *p, size_t s )
{
	void *rv = realloc( p, s );
	if( rv == NULL ) {
		perror("realloc"); /* uncovered */
		exit(1);           /* uncovered */
	}
	return rv;
}

void
die(const char *msg)  /* uncovered */
{
	perror(msg);  /* uncovered */
	exit(1);      /* uncovered */
}                     /* uncovered */
