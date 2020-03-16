/*
 * A simple reverse polish calculator
 *
 * '-' is treated specially.  When possible, it is treated as a numeric symbol,
 * so you can use '2 -1+' or '2 1-' to subtract 1 from 2.  This makes it easier
 * to enter negative numbers and values like '1e-2'.  Any time a token can be
 * interpreted as a numeric value, it is.  This sometimes gets confusing.  For
 * example, '3 1--5-' will compute (3-1)-(-5), pushing 7 onto the stack.
 * '4,1-8' will simply push 3 values onto the stack, while '4,1- 8' will subtract
 * 1 from 4, leaving 3 and 8 on the stack.
 *
 * ',' is used to separate entries.  So '1,2+' computes the sum of 1 and 2
 * '_' is an ignored place holder, so 65536 can be written 65_536
 */

/*
 * TODO:
 * document x better, and generate an error if 'x' is given and the
 * register is empty.  eg, '[1 1+]x' is supposed to execute 1 1+, and
 * 3x should execute the contents of register 3, but no error is
 * given if either register is empty.

 */

#include "stack.h"
#include "ring-buffer.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

#define numeric_tok "-0123456789XPEabcdef."
#define string_ops "[]FxLRD"
#define binary_ops "*+/^r"
#define unary_ops "lknpy"
#define nonary_ops "hq_"
#define token_div " \t\n,"

void print_help( void ) {
	puts(
		"D    Delete the first register\n"
		"F    use value from the string stack as format string\n"
		"[s]  push s onto the string stack\n"
		"h    print this help message\n"
		"k    set precision of %g format string\n"
		"l    list elements of the stack\n"
		"L    list elements of string stack\n"
		"n    print and pop top value of stack\n"
		"p    print top value of stack\n"
		"q    quit\n"
		"r    swap top two elements of the stack\n"
		"R    swap top two registers\n"
		"x    execute string in string stack\n"
		"y    duplicate top value of stack\n"
	);
}

struct state {
	struct stack *stack;
	struct stack *registers;
	char fmt[32];
	int enquote;
	struct ring_buf *r;
};

void process_entry( struct state *S, unsigned char c );
void push_it( struct state *S, unsigned char c );
void apply_binary( struct state *S, unsigned char c );
void apply_unary( struct state *S, unsigned char c );
void apply_string_op( struct state *S, unsigned char c );
void die( const char *msg );
void write_args_to_stdin( char *const*argv );
static int push_value(struct state *, unsigned char);


int
main( int argc, char **argv )
{
	int c;
	struct state S[1];
	struct ring_buf *B;

	S->r = rb_create( 32 );
	S->enquote = 0;
	S->stack = stack_xcreate(sizeof(long double));
	S->registers = stack_xcreate(0);
	B = rb_create(32);
	stack_xpush(S->registers, B);
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
	struct ring_buf *b = stack_get(S->registers, -1);

	if( S->enquote && c != ']' ) {
		rb_push(b, c);
	} else if( strchr( numeric_tok, c )) {
		rb_push(b, c);
	} else if( strchr( string_ops, c )) {
		apply_string_op( S, c );
	} else if( strchr( token_div, c )) {
		push_value(S, c);
	} else if(strchr( binary_ops, c )) {
		if(!push_value(S, c))
			apply_binary(S, c);
	} else if(strchr( unary_ops, c )) {
		if(!push_value(S, c))
			apply_unary(S, c);
	} else switch(c) {
		case '_': break; /* noop */
		case 'q': exit(0);
		case 'h': print_help();
		default: fprintf( stderr, "Unexpected: %c\n", c );
	}
}


/*
 * Parse a number.  If we encounter an unexpcted '-',
 * treat it as a binary operator and push the rest of
 * the buffer back on the stack to be processed, and
 * return non-zero to indicate that has happened.
 */
int
push_value(struct state *S, unsigned char c)
{
	struct ring_buf *b = stack_get(S->registers, -1);
	char s[256] = "", *end = s + sizeof s;
	char *cp, *start;
	int i;

	cp = start = s;
	if(! rb_isempty(b)) {
		long double val;

		while( (i = rb_pop(b)) != EOF) {
			if(cp < end) {
				*cp++ = i;
			}
		}
		if(cp == end) {
			fprintf(stderr, "Overflow: Term truncated\n", s);
			return 0;
		}
		val = strtold(s, &cp);
		while( *cp == '-' && cp != start) {
			stack_push(S->stack, &val);
			start = cp;
			val = strtold(start, &cp);
		}
		if( *cp == '-' ) {
			apply_binary( S, '-' );
			for( char *t = cp + 1; *t; t++ ) {
				push_it(S, *t);
			}
			push_it(S, c);
		} else if( *cp ) {
			fprintf(stderr, "Garbled (discarded): %s\n", s);
		} else {
			stack_push(S->stack, &val);
		}
	}
	return *cp == '-';
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
	struct ring_buf *ret = NULL;
	int offset;
	struct ring_buf *b = stack_get(S->registers, -1);

	if( rb_isempty(b)) {
		offset = stack_size(S->registers) - 2;
	} else {
		offset = rb_tail(b) - '0';
	}
	if( offset < 0 || offset > stack_size(S->registers) - 2 ) {
		fprintf(stderr, "Invalid register\n");
	} else {
		ret = stack_get(S->registers, offset);
	}

	return ret;
}

void
apply_string_op( struct state *S, unsigned char c )
{
	struct ring_buf *Bp;
	switch(c) {
	case '[':
		push_value(S, c);
		S->enquote = 1;
		break;
	case ']':
		S->enquote = 0;
		stack_xpush(S->registers, rb_create(32));
		break;
	case 'D':
		if( stack_size(S->registers) > 1 ) {
			void *e;
			struct ring_buf *a, *b;
			a = stack_pop(S->registers, &e);
			b = stack_pop(S->registers, &e);
			stack_push(S->registers, a);
		}
	break;
	case 'F': {
		struct ring_buf *rb = select_char_buf(S);
		if( rb ) {
			int c, j;
			char *b = S->fmt;
			for( j = 0; (c = rb_peek(rb, j)) != EOF && j < sizeof S->fmt; j++) {
				*b++ = c;
			}
			*b = '\0';
			validate_format( S );
		}
	} break;
	case 'R':
		if( stack_size(S->registers) > 2 ) {
			void *e;
			struct ring_buf *a, *b, *c;
			a = stack_pop(S->registers, &e);
			b = stack_pop(S->registers, &e);
			c = stack_pop(S->registers, &e);
			stack_push(S->registers, b);
			stack_push(S->registers, c);
			stack_push(S->registers, a);
		}
	break;
	case 'L': {
		for( int i = 0; i < stack_size(S->registers) - 1; i++ ) {
			int j = 0, c;
			struct ring_buf *s = stack_get(S->registers, i);

			printf("(%d): ", i);
			while( (c = rb_peek(s, j++)) != EOF ) {
				putchar(c);
			}
			putchar('\n');
		}
	} break;
	case 'x': {
		struct ring_buf *rb = select_char_buf(S);
		if( rb ) {
			int j=0, c;
			while( (c = rb_peek(rb, j++)) != EOF ) {
				rb_push( S->r, c );
			}
		}
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
	assert( strchr( binary_ops, c ) || c == '-' );
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
	case '-': res = val[1] - val[0]; break;
	case '+': res = val[1] + val[0]; break;
	case '/': res = val[1] / val[0]; break;
	case '^': res = pow(val[1], val[0]); break;
	}
	stack_push(S->stack, &res);
}
