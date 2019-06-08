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
#include "ring-buffer.h"

#define string_ops "[]Fxl"
#define binary_ops "*+/^-r"
#define unary_ops "dfkp"
#define nonary_ops "hq"
#define token_div " \t\n"

void print_help( void ) {
	puts(
		"F    use value from the string stack as format string\n"
		"[s]  add s to the string stack\n"
		"x    execute string in string stack\n"
		"l    list elements of string stack\n"
		"d    duplicate top value of stack\n"
		"f    print contents of stack\n"
		"k    set precision of %g format string\n"
		"p    print top value of stack\n"
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

void process_entry( struct state *S, int c );
void apply_binary( struct state *S, int c );
void apply_unary( struct state *S, int c );
void apply_string_op( struct state *S, int c );
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

	S->r = rb_create();
	S->enquote = 0;
	S->cb_size = S->stack_size = 4;
	S->sp = S->stack = xrealloc( NULL, sizeof *S->sp * S->stack_size );
	S->cbp = S->char_stack = xrealloc( NULL, sizeof *S->cbp * S->cb_size );
	init_char_buf( S->cbp );
	strcpy( S->fmt, "%.3Lg\n" );

	if( argc > 1) {
		for( ; *++argv; process_entry( S, ' ')) {
			for( char *t = *argv; *t; t++ ) {
				rb_push( S->r, *t );
				while(( c = rb_pop( S->r )) != EOF ) {
					process_entry( S, c );
				}
			}
		}
	} else while( (c=getchar()) != EOF ) {
		int d;
		rb_push( S->r, c );
		while(( d = rb_pop( S->r )) != EOF ) {
			process_entry( S, d );
		}
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
	if( S->enquote && c != ']' ) {
		push_buf( S, c );
	} else if( strchr( string_ops, c )) {
		apply_string_op( S, c );
	} else if( strchr( token_div, c )) {
		push_number(S);
	} else if(strchr( nonary_ops, c )) {
		switch(c) {
		case 'q': exit(0);
		case 'h': print_help();
		}
	} else if(strchr( unary_ops, c )) {
		push_number(S);
		apply_unary(S, c);
	} else if(strchr( binary_ops, c )) {
		push_number(S);
		apply_binary(S, c);
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
validate_format( struct state const *S )
{
	int count[2] = {0};
	for(char const *k = S->fmt; *k; k++) {
		if( *k == '%' )
			count[0] += 1;
		if( *k == 'L' && count[0] )
			count[1] += 1;
	}
	if( ! count[0] || ! count[1] ) {
		fputs( "Warning: output fmt should match '%.*L'\n", stderr );
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
apply_string_op( struct state *S, int c )
{
	switch(c) {
	case '[':
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
	case 'l':
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
apply_unary( struct state *S, int c )
{
	assert( S->sp >= S->stack );
	assert( strchr( unary_ops, c ));
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
	}
}


void
apply_binary(struct state *S, int c)
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

void init_char_buf( struct char_buf *p )
{
	p->size = 32;
	p->bp = p->buf = xrealloc( NULL, sizeof *p->buf * p->size );
}
