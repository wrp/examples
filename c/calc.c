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

#define binary "*+/^-r"
#define unary "dfkpq"

struct char_buf {
	char *buf, *bp;
	size_t size;
};

struct state {
	long double *stack, *sp;
	size_t stack_size;
	struct char_buf *char_stack, *cbp;
	size_t cb_size;
	char fmt[32];
};

void process_entry( struct state *S, int c );
void apply_binary( struct state *S, int c );
void apply_unary( struct state *S, int c );
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

	S->cb_size = S->stack_size = 4;
	S->sp = S->stack = xrealloc( NULL, sizeof *S->sp * S->stack_size );
	S->cbp = S->char_stack = xrealloc( NULL, sizeof *S->cbp * S->cb_size );
	S->cbp->size = 1;
	S->cbp->bp = S->cbp->buf = xrealloc( NULL, sizeof *S->cbp->buf * S->cbp->size );
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
	*S->cbp->bp++ = (char)c;
	if( S->cbp->bp == S->cbp->buf + S->cbp->size ) {
		S->cbp->buf = xrealloc( S->cbp->buf, S->cbp->size * 2 * sizeof *S->cbp->buf );
		S->cbp->bp = S->cbp->buf + S->cbp->size;
		S->cbp->size *= 2;
	}
}

void
process_entry( struct state *S, int c )
{
	if( strchr( " \t\n", c )) {
		push_number(S);
	} else if(strchr( binary, c )) {
		push_number(S);
		apply_binary(S, c);
	} else if(strchr( unary, c )) {
		push_number(S);
		apply_unary(S, c);
	} else {
		push_buf(S, c);
	}
}


void
push_number( struct state *S )
{
	*S->cbp->bp = '\0';
	if( S->cbp->bp != S->cbp->buf ) {
		char *end;
		*(S->sp++) = strtold(S->cbp->buf, &end);
		if( end != S->cbp->bp ) {
			fprintf(stderr, "Garbled: %s\n", S->cbp->buf);
		}
		S->cbp->bp = S->cbp->buf;
		if( S->sp == S->stack + S->stack_size ) {
			grow_stack(S);
		}
	}
}


void
apply_unary( struct state *S, int c )
{
	assert( S->sp >= S->stack );
	assert( strchr( unary, c ));
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
apply_binary(struct state *S, int c)
{
	assert( S->sp >= S->stack );
	assert( strchr( binary, c ));
	if( S->sp - S->stack < 2 ) {
		fputs( "Stack empty (need 2 values)\n", stderr );
		return;
	}
	S->sp -= 1;
	switch(c) {
	case 'r': {
		typeof (*S->sp) tmp = S->sp[0];
		S->sp[0] = S->sp[-1];
		S->sp[-1] = tmp;
		S->sp += 1;
	} break;
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
