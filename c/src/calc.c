const char *help[] = {
"A simple reverse polish calculator",
"",
"'-' is treated specially.  When possible, it is treated as a numeric symbol,",
"so you can use '2 -1+' or '2 1-' to subtract 1 from 2.  This makes it easier",
"to enter negative numbers and values like '1e-2'.  Any time a token can be",
"interpreted as a numeric value, it is.  This sometimes gets confusing.  For",
"example, '3 1--5-' will compute (3-1)-(-5), pushing 7 onto the stack.",
"'4,1-8' will simply push 3 values onto the stack, while '4,1- 8' will",
"subtract 1 from 4, leaving 3 and 8 on the stack.",
"",
"',' is used to separate entries.  So '1,2+' computes the sum of 1 and 2",
"'_' is an ignored place holder, so 65536 can be written 65_536",
"",
"All computation is done using double precision floats, but you can",
"control the output format with k or F.  Although values are parsed",
"using strtod, you must use X and P verses x and p.",
"eg, 0X34P3 will be parsed as the hex value 34 shifted left 3 bits,",
"but 0x34p3 will place 0 on the stack, try to execute the ",
"string in register 0, push decimal 34 on the stack, print it,",
"and then push 3 onto the stack.",
0
};


#include "stack.h"
#include "ring-buffer.h"
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

#define COMMA_DEFAULT_FMT "%.3'Lg\n"
#define DEFAULT_FMT "%.3Lg\n"
#define numeric_tok "-0123456789XPEabcdef."
#define string_ops "[]D~FRxZ"
#define binary_ops "*+/^r"
#define unary_ops "knpyY"
#define nonary_ops "hq_"
#define token_div " \t\n,"

struct state {
	struct stack *values;
	struct stack *registers;
	char fmt[32];
	int enquote;
	struct ring_buf *raw;   /* raw input as entered */
	struct ring_buf *accum; /* accumulator (used to re-process) */
	enum { rational, integer } type;
};

struct func {
	const char *name;
	union {
		long double (*f)(long double);
		long double (*g)(long double, long double);
	} f;
} unary_functions[] = {
	{ "acos", acosl },
	{ "asin", asinl },
	{ "atan", atanl },
	{ "cos", cosl },
	{ "cosh", coshl },
	{ "log", logl },
	{ "log10", log10l },
	{ "log1p", log1pl },
	{ "log2", log2l },
	{ "sin", sinl },
	{ "sinh", sinhl },
	{ "sqrt", sqrtl },
	{ "tan", tanl },
	{ "tanh", tanhl },
	{ NULL, 0 },
};

struct func binary_functions[] = {
	{ "pow", .f.g = powl},
	{ NULL, 0 },
};

static void show_functions(void);

void
print_help(struct state *S)
{
	for( const char **s = help; *s; s++ ){
		puts(*s);
	}
	putchar('\n');
	puts(
		"~    use function from specified register\n"
		"D    Delete the first register\n"
		"F    use value from the specified register as format string\n"
		"[s]  push s onto the register stack\n"
		"h    print this help message\n"
		"k    set precision of format string\n"
		"n    print and pop top value of stack\n"
		"p    print top value of stack\n"
		"q    quit\n"
		"r    swap top two elements of the stack\n"
		"R    swap top two elements of the register stack\n"
		"x    execute a string in register\n"
		"y    duplicate top value of stack\n"
		"Y    list (examine) elements of the stack\n"
		"Z    list elements of register stack\n"
	);
	puts("\nThe ~ command understands the following functions:");
	show_functions();
	fprintf(stderr, "Output format currently: %s", S->fmt);
}

void process_entry(struct state *S, unsigned char c);
void push_it(struct state *, int);
void apply_binary(struct state *S, unsigned char c);
void apply_unary(struct state *S, unsigned char c);
void apply_string_op(struct state *S, unsigned char c);
void die(const char *msg);
void write_args_to_stdin(char *const*argv);
static int push_value(struct state *, unsigned char);
struct ring_buf * select_register(struct state *S);

static void
init_state(struct state *S)
{
	S->raw = rb_create(32);
	S->accum = rb_create(32);
	S->enquote = 0;
	S->type = rational;
	S->values = stack_xcreate(sizeof(long double));
	S->registers = stack_xcreate(0);
	strcpy(S->fmt, DEFAULT_FMT);
}

int
main(int argc, char **argv)
{
	int c;
	struct state S[1];

	setlocale(LC_NUMERIC, "");
	init_state(S);

	if( argc > 1 ){
		for( ; *++argv; push_it(S, ' ') ){
			for( char *t = *argv; *t; t++ ){
				push_it(S, *t);
			}
		}
	} else while( (c=getchar()) != EOF ){
		push_it(S, c);
	}
	rb_free(S->raw);
	rb_free(S->accum);
	return 0;
}


/*
 * Push an item onto the ring buffer and then read the ring buffer until empty.
 * Some operations (notably 'x') will push additional values into the ring
 * buffer, and they should be processed before the next entry coming from input.
 */
void
push_it(struct state *S, int c)
{
	int k;
	rb_push(S->raw, (unsigned char)c);
	while( (k = rb_pop( S->raw )) != EOF ){
		process_entry(S, (unsigned char)k);
	}
}


void
process_entry(struct state *S, unsigned char c)
{
	struct ring_buf *b = S->accum;

	if( S->enquote && c != ']' ){
		rb_push(b, c);
	} else if( strchr(numeric_tok, c) ){
		rb_push(b, c);
	} else if( strchr(string_ops, c) ){
		apply_string_op(S, c);
	} else if( strchr(token_div, c) ){
		push_value(S, c);
	} else if( strchr(binary_ops, c) ){
		if( !push_value(S, c) ){
			apply_binary(S, c);
		}
	} else if( strchr(unary_ops, c) ){
		if( !push_value(S, c) ){
			apply_unary(S, c);
		}
	} else switch( c ) {
		default: fprintf( stderr, "Unexpected: %c\n", c );
		case '_': break; /* noop */
		case 'q': exit(0);
		case 'h':
		case '?': print_help(S);
	}
}

/*
 * Parse a number.  If we encounter an unexpected '-',
 * treat it as a binary operator and push the rest of
 * the buffer back on the stack to be processed, and
 * return non-zero to indicate that has happened.
 */
int
push_value(struct state *S, unsigned char c)
{
	struct ring_buf *b = S->accum;
	char s[256] = "", *end = s + sizeof s;
	char *cp, *start;
	int i;

	cp = start = s;
	if( ! rb_isempty(b) ){
		long double val;

		while( (i = rb_pop(b)) != EOF ){
			if( cp < end ){
				*cp++ = i;
			}
		}
		if( cp == end ){
			fprintf(stderr, "Overflow: Term truncated\n");
			return 0;
		}
		val = strtold(start, &cp);
		while( *cp == '-' && cp != start ){
			stack_push(S->values, &val);
			start = cp;
			val = strtold(start, &cp);
		}
		if( *cp == '-' ){
			assert( cp == start );
			apply_binary(S, '-');
			for( char *t = cp + 1; *t; t++ ){
				push_it(S, *t);
			}
			push_it(S, c);
		} else if( *cp ){
			fprintf(stderr, "Garbled (discarded): %s\n", s);
		} else {
			stack_push(S->values, &val);
		}
	}
	return *cp == '-';
}


static void
show_functions(void)
{
	struct func *funcs[2] = { unary_functions, binary_functions };;
	for( int i = 0; i < 2; i += 1 ){
		struct func *func = funcs[i];
		putchar('\t');
		while( func->name ){
			printf("%s", func->name);
			func += 1;
			if( func->name ){
				fputs(", ", stdout);
			}
		}
		putchar('\n');
	}
}


static void
execute_function(struct state *S, const char *cmd)
{
	long double arg;
	long double res;
	struct func *func = unary_functions;

	stack_pop(S->values, &res);

	/* TODO: do a lookup via polynomial rolling hash instead
	* of this absurd linear search via strcmp.
	*/
	for ( ; func->name; func += 1 ){
		if (strcmp(cmd, func->name) == 0) {
			res = func->f.f(res);
			goto done;
		}
	}

	for ( func = binary_functions; func->name; func += 1 ){
		if (strcmp(cmd, func->name) == 0) {
			stack_pop(S->values, &arg);
			res = func->f.g(arg, res);
			break;
		}
	}

done:
	if (func->name == NULL) {
		fprintf(stderr, "Unknown function: %s\n", cmd);
	}

	stack_push(S->values, &res);
}


static void
apply_function(struct state *S)
{
	struct ring_buf *rb = select_register(S);
	if( ! rb ){
		return;
	}
	char cmd[128];
	int c;
	char *b = cmd;
	char *e = cmd + sizeof cmd - 1;
	while( b < e && (c = rb_peek(rb, b - cmd)) != EOF ){
		*b++ = c;
	}
	*b = '\0';
	execute_function(S, cmd);
}


void
extract_format(struct state *S)
{
	struct ring_buf *rb = select_register(S);
	if( rb ){
		char *b = S->fmt, *e = S->fmt + sizeof S->fmt;
		int count = 0;
		int c = 0;

		while( b < e && (c = rb_peek(rb, b - S->fmt)) != EOF ){
			/* Extremely naive check of format string.  */
			*b++ = c;
			if( count == 2 ){
				switch( c ){
				case 'd':
				case 'i':
				case 'o':
				case 'u':
				case 'x':
				case 'X':
					S->type = integer;
					break;
				case 'f':
				case 'e':
				case 'g':
				case 'a':
					S->type = rational;

				}
			}
			count += !count && c == '%';
			count += count && c == 'L';
		}
		if( b > S->fmt && b + 1 < e && b[-1] != '\n' ){
			*b++ = '\n';
		}
		*b = '\0';
		if( count < 2 ){
			fputs("Warning: output fmt should print a long value "
				"(eg '\%Lf')\n", stderr);
		}
	}
}

struct ring_buf *
select_register(struct state *S)
{
	long double val = -1.0;
	struct ring_buf *ret = NULL;
	int offset = -1;

	if( stack_pop(S->values, &val) ){
		offset = val;
	}
	if( rint(val) != val ){
		fprintf(stderr, "Invalid register: %Lg", val);
		fprintf(stderr, " is not an integer\n");
		stack_push(S->values, &val);
	} else if( (ret = stack_get(S->registers, offset)) == NULL ){
		if( offset == -1 ){
			fprintf(stderr, "Register stack empty\n");
		} else {
			fprintf(stderr, "Register %d empty\n", offset);
		}
	}

	return ret;
}

void
apply_string_op(struct state *S, unsigned char c)
{
	struct ring_buf *rb;
	void *e;
	assert( !S->enquote || c == ']' );
	if( c != ']' ){
		push_value(S, c);
	}
	switch( c ){
	case '[':
		S->enquote = 1;
		break;
	case ']':
		S->enquote = 0;
		stack_xpush(S->registers, S->accum);
		S->accum = rb_create(32);
		break;
	case '~':
		apply_function(S);
		break;
	case 'D':
		if( stack_size(S->registers) > 0 ){
			stack_pop(S->registers, &e);
		}
		break;
	case 'F':
		extract_format(S);
		break;
	case 'R':
		if( stack_size(S->registers) > 1 ){
			struct ring_buf *a, *b;
			stack_pop(S->registers, &a);
			stack_pop(S->registers, &b);
			stack_push(S->registers, a);
			stack_push(S->registers, b);
		}
		break;
	case 'Z':
		for( unsigned i = 0; i < stack_size(S->registers); i++ ){
			int j = 0, c;
			struct ring_buf *s = stack_get(S->registers, i);

			printf("(%d): ", i);
			while( (c = rb_peek(s, j++)) != EOF ){
				putchar(c);
			}
			putchar('\n');
		}
		break;
	case 'x':
		if( (rb = select_register(S)) != NULL ){
			int j=0, c;
			while( (c = rb_peek(rb, j++)) != EOF ){
				rb_push(S->raw, c);
			}
		}
		break;
	}
}

void
print_stack(struct state *S)
{
	unsigned i = 0;
	long double *s;
	for( s = stack_base(S->values); i < stack_size(S->values); s++, i++ ){
		printf("%3u: ", i);
		printf(S->fmt, *s);
	}
}


void
apply_unary(struct state *S, unsigned char c)
{
	long double val;
	assert( strchr(unary_ops, c) );
	if( !stack_pop(S->values, &val) ){
		fputs("Stack empty (need 1 value)\n", stderr);
		return;
	}
	switch( c ){
	case 'y':
		stack_push(S->values, &val);
		stack_push(S->values, &val);
		break;
	case 'k':
		if( val < 1 ){
			snprintf(S->fmt, sizeof S->fmt, "%%'Ld\n");
			S->type = integer;
		} else {
			snprintf(S->fmt, sizeof S->fmt, "%%.%dLf\n", (int)val);
			S->type = rational;
		}
		break;
	case 'Y':
		stack_push(S->values, &val);
		print_stack(S);
		break;
	case 'p': stack_push(S->values, &val); /* Fall thru */
	case 'n':
		if( S->type == rational ){
			printf(S->fmt, val);
		} else {
			long lval = (long)val;
			printf(S->fmt, lval);
		}
		break;
	}
}

void
apply_binary(struct state *S, unsigned char c)
{
	long double val[2];
	long double res;
	assert( strchr(binary_ops, c) || c == '-' );
	if( stack_size(S->values) < 2 ){
		fputs("Stack empty (need 2 values)\n", stderr);
		return;
	}
	stack_pop(S->values, val);
	stack_pop(S->values, val + 1);
	switch(c) {
	case 'r':
		stack_push(S->values, val);
		res = val[1];
		break;
	case '*': res = val[1] * val[0]; break;
	case '-': res = val[1] - val[0]; break;
	case '+': res = val[1] + val[0]; break;
	case '/': res = val[1] / val[0]; break;
	case '^': res = pow(val[1], val[0]); break;
	}
	stack_push(S->values, &res);
}
