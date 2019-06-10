/*
 * A simple reverse polish calculator
 *
 * '-' is not an operator.  Instead, it is a numeric symbol.  To perform
 * simple arithmetic, you must use +.  For example, '2 -1+' to subtract 1
 * from 2.  This makes it easier to enter negative numbers.  To
 * enter -5 you just enter '-5' instead of '0 5-' and .01 is '1e-2' (or '.01'!)
 * rather than the cumbersome '1 0 * 2-^')
 *
 * ',' is used to separate entries.  So '1,2+' computes the sum of 1 and 2
 * '_' is an ignored place holder, so 65536 can be written 65_536
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "ring-buffer.h"

#define numeric_tok "-0123456789XPABCDEabcde."
#define string_ops "[]FxL"
#define binary_ops "*+/^r"
#define unary_ops "lknpy"
#define nonary_ops "hq_"
#define token_div " \t\n,"

void print_help( void ) {
	puts(
		"F    use value from the string stack as format string\n"
		"[s]  push s onto the string stack\n"
		"h    print this help message\n"
		"k    set precision of %g format string\n"
		"l    list elements of the stack\n"
		"L    list elements of string stack\n"
		"n    print and pop top value of stack\n"
		"p    print top value of stack\n"
		"q    quite"
		"x    execute string in string stack\n"
		"y    duplicate top value of stack\n"
	);
}

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
	int enquote;
	struct ring_buf *r;
};

void process_entry( struct state *S, unsigned char c );
void push_it( struct state *S, unsigned char c );
void apply_binary( struct state *S, unsigned char c );
void apply_unary( struct state *S, unsigned char c );
void apply_string_op( struct state *S, unsigned char c );
void grow_stack( struct state *S );
void * xrealloc( void *p, size_t s );
void die( const char *msg );
void write_args_to_stdin( char *const*argv );
void push_number( struct state *S );
void init_char_buf( struct char_buf *p );

int
main( int argc, char **argv )
{
	int c;
	struct state S[1];

	S->r = rb_create( 32 );
	S->enquote = 0;
	S->cb_size = S->stack_size = 4;
	S->sp = S->stack = xrealloc( NULL, sizeof *S->sp * S->stack_size );
	S->cbp = S->char_stack = xrealloc( NULL, sizeof *S->cbp * S->cb_size );
	init_char_buf( S->cbp );
	strcpy( S->fmt, "%.3Lg\n" );

	if( argc > 1) {
		for( ; *++argv; push_it( S, (unsigned char)' ')) {
			for( char *t = *argv; *t; t++ ) {
				push_it( S, (unsigned char)*t );
			}
		}
	} else while( (c=getchar()) != EOF ) {
		push_it( S, (unsigned char)c );
	}
	return 0;
}


/*
 * Append an octet to the char buf.   Grow if necessary.
 */
void
push_buf( struct state *S, unsigned char c )
{
	*S->cbp->bp++ = (char)c;
	if( S->cbp->bp == S->cbp->buf + S->cbp->size ) {
		S->cbp->buf = xrealloc( S->cbp->buf, S->cbp->size * 2 * sizeof *S->cbp->buf );
		S->cbp->bp = S->cbp->buf + S->cbp->size;
		S->cbp->size *= 2;
	}
}


/*
 * Push an item onto the ring buffer and then read the ring buffer until empty.
 * Some operations (notably 'x') will push additional values into the ring
 * buffer, and they should be processed before the next entry coming from input.
 */
void
push_it( struct state *S, unsigned char c )
{
	int k;
	rb_push( S->r, c );
	while( ( k = rb_pop( S->r )) != EOF ) {
		process_entry( S, (unsigned char)k );
	}
}


void
process_entry( struct state *S, unsigned char c )
{

	if( S->enquote && c != ']' ) {
		push_buf( S, c );
	} else if( strchr( numeric_tok, c )) {
		push_buf( S, c );
	} else if( strchr( string_ops, c )) {
		apply_string_op( S, c );
	} else if( strchr( token_div, c )) {
		push_number( S );
	} else if(strchr( binary_ops, c )) {
		push_number( S );
		apply_binary( S, c );
	} else if(strchr( nonary_ops, c )) {
		switch(c) {
		case '_': break; /* noop */
		case 'q': exit(0);
		case 'h': print_help();
		}
	} else if(strchr( unary_ops, c )) {
		push_number( S );
		apply_unary( S, c );
	} else {
		fprintf( stderr, "Unexpected: %c\n", c );
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


/*
 * Extremely naive check of format string.
 * Primarily this is to remind the user to include 'L',
 * but doesn't catch simple mistakes like '%L'
 */
void
validate_format( struct state const *S )
{
	int count[2] = {0};
	for(char const *k = S->fmt; *k; k++) {
		count[0] += *k == '%';
		count[1] += count[0] && *k == 'L';
	}
	if( ! count[0] || ! count[1] ) {
		fputs( "Warning: output fmt should print a long double (eg '%%Lf')\n", stderr );
	}
}


char *
select_char_buf( struct state *S )
{
	int offset;
	if( S->cbp->bp != S->cbp->buf ) {
		offset = *--S->cbp->bp - '0';
	} else {
		offset = S->cbp - S->char_stack - 1;
	}
	if( offset < 0 || offset > ( S->cbp - S->char_stack - 1 )) {
		fprintf(stderr, "Invalid register\n");
		return NULL;
	}
	return S->char_stack[offset].buf;
}

void
apply_string_op( struct state *S, unsigned char c )
{
	switch(c) {
	case '[':
		push_number( S );
		S->enquote = 1;
		break;
	case ']':
		S->enquote = 0;
		*S->cbp->bp = '\0';
		S->cbp += 1;
		if( S->cbp == S->char_stack + S->cb_size ) {
			S->char_stack = xrealloc(S->char_stack, S->cb_size * 2 * sizeof *S->cbp );
			S->cbp = S->char_stack + S->cb_size;
			S->cb_size *= 2;
		}
		init_char_buf( S->cbp );
		break;
	case 'F':
		snprintf(S->fmt, sizeof S->fmt, "%s\n", select_char_buf( S ) ? : "%.3Lg" );
		validate_format( S );
		break;
	case 'L':
		for( typeof(S->cbp) s = S->char_stack; s < S->cbp; s++ ) {
			printf("(%d): %s\n", (int)(s - S->char_stack), s->buf );
		}
		break;
	case 'x':
		for( char *k = select_char_buf( S ); k && *k; k++ ) {
			rb_push( S->r, *k );
		}
		break;
	}
}


void
apply_unary( struct state *S, unsigned char c )
{
	assert( S->sp >= S->stack );
	assert( strchr( unary_ops, c ));
	if( S->sp - S->stack < 1 ) {
		fputs( "Stack empty (need 1 value)\n", stderr );
		return;
	}
	switch(c) {
	case 'y':
		S->sp[0] = S->sp[-1];
		S->sp += 1;
		break;
	case 'k':
		snprintf(S->fmt, sizeof S->fmt, "%%.%dLg\n", (int)*--S->sp);
		break;
	case 'l':
		for(typeof(S->stack) s = S->stack; s < S->sp; s++) {
			printf("%3u: ", (unsigned)(s - S->stack));
			printf(S->fmt, *s);
		}
		break;
	case 'p':
		printf(S->fmt, S->sp[-1]);
		break;
	case 'n':
		printf(S->fmt, *--S->sp);
		break;
	}
}


void
apply_binary(struct state *S, unsigned char c)
{
	assert( S->sp >= S->stack );
	assert( strchr( binary_ops, c ));
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

void init_char_buf( struct char_buf *p )
{
	p->size = 32;
	p->bp = p->buf = xrealloc( NULL, sizeof *p->buf * p->size );
}
