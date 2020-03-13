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

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "ring-buffer.h"
#include "stack.h"

#define numeric_tok "-0123456789XPEabcdef."
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
		"q    quit\n"
		"x    execute string in string stack\n"
		"y    duplicate top value of stack\n"
	);
}

struct char_buf {
	struct ring_buf *r;
};


struct state {
	struct stack *stack;
	struct stack *char_stack;
	char fmt[32];
	int enquote;
	struct ring_buf *r;
};

void process_entry( struct state *S, unsigned char c );
void push_it( struct state *S, unsigned char c );
void apply_binary( struct state *S, unsigned char c );
void apply_unary( struct state *S, unsigned char c );
void apply_string_op( struct state *S, unsigned char c );
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
	S->stack = stack_create(sizeof(long double));
	S->char_stack = stack_create(sizeof(struct char_buf));
	init_char_buf(stack_top(S->char_stack));
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
	struct char_buf* cbp = stack_top(S->char_stack);;
	rb_push(cbp->r, c);
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
	struct char_buf *cbp = stack_top(S->char_stack);

	if(! rb_isempty(cbp->r)) {
		long double val;
		char s[128];
		char *cp = s;

		rb_push(cbp->r, '\0');
		while( (*cp++ = rb_pop(cbp->r)) != EOF)
			;

		val = strtold(s, &cp);
		if( *cp ) {
			fprintf(stderr, "Garbled: %s\n", s);
		}
		stack_push(S->stack, &val);
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


struct ring_buf *
select_char_buf( struct state *S )
{
	int offset;
	struct char_buf *cbp = stack_top(S->char_stack);
	if( rb_isempty(cbp->r)) {
		offset = cbp - (typeof(cbp))stack_base(S->char_stack) - 1;
	} else {
		offset = rb_tail(cbp->r) - '0';
	}
	if( offset < 0 || offset > ( cbp - (typeof(cbp))stack_base(S->char_stack) - 1 )) {
		fprintf(stderr, "Invalid register\n");
		return NULL;
	}

	return ((typeof(cbp))stack_base(S->char_stack))[offset].r;
}

void
apply_string_op( struct state *S, unsigned char c )
{
	struct char_buf *cbp = stack_top(S->char_stack);
	int i = 0;
	switch(c) {
	case '[':
		push_number( S );
		S->enquote = 1;
		break;
	case ']':
		S->enquote = 0;
		rb_push(cbp->r, '\0');
		cbp = stack_incr(S->char_stack);
		init_char_buf( cbp );
		break;
	case 'F': {
		char buf[32];
		struct ring_buf *rb = select_char_buf(S);
		rb_string(rb, buf, sizeof buf);
		snprintf(S->fmt, sizeof S->fmt, "%s\n", *buf ? buf : "%.3Lg" );
		validate_format( S );
	} break;
	case 'L': {
		for( typeof(cbp) s = stack_base(S->char_stack); i < stack_size(S->char_stack); s++, i++ ) {
			char *buf = malloc(rb_length(s->r) + 4);
			rb_string(s->r, buf, rb_length(s->r));
			printf("(%d): %s\n", i, buf);
			free(buf);
		}
	} break;
	case 'x': {
		struct ring_buf *rb = select_char_buf(S);
		char *buf = malloc(rb_length(rb) + 4);
		rb_string(rb, buf, rb_length(rb));
		for( char *k = buf; k && *k; k++ ) {
			rb_push( S->r, *k );
		}
		free(buf);
	} break;
	}
}


void
apply_unary( struct state *S, unsigned char c )
{
	long double val;
	unsigned i = 0;
	assert( strchr( unary_ops, c ));
	if( stack_size(S->stack) < 1 ) {
		fputs( "Stack empty (need 1 value)\n", stderr );
		return;
	}
	stack_pop(S->stack, &val);
	switch(c) {
	case 'y':
		stack_push(S->stack, &val);
		stack_push(S->stack, &val);
		break;
	case 'k':
		snprintf(S->fmt, sizeof S->fmt, "%%.%dLg\n", (int)val);
		break;
	case 'l':
		stack_push(S->stack, &val);
		for(long double * s = stack_base(S->stack); i < stack_size(S->stack); s++, i++) {
			printf("%3u: ", i);
			printf(S->fmt, *s);
		}
		break;
	case 'p':
		stack_push(S->stack, &val);
	case 'n':
		printf(S->fmt, val);
		break;
	}
}


void
apply_binary(struct state *S, unsigned char c)
{
	long double val[2];
	long double res;
	assert( strchr( binary_ops, c ));
	if( stack_size(S->stack) < 2 ) {
		fputs( "Stack empty (need 2 values)\n", stderr );
		return;
	}
	stack_pop(S->stack, val);
	stack_pop(S->stack, val + 1);
	switch(c) {
	case 'r': {
		stack_push(S->stack, val);
		res = val[1];
	} break;
	case '*': res = val[1] * val[0]; break;
	case '+': res = val[1] + val[0]; break;
	case '/': res = val[1] / val[0]; break;
	case '^': res = pow(val[1], val[0]); break;
	}
	stack_push(S->stack, &res);
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

void
init_char_buf( struct char_buf *p )
{
	p->r = rb_create(32);
}
