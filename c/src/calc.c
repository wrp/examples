const char *help[] = {
"A simple reverse polish calculator",
"",
"When possible, '-' and '+' are treated as numeric symbols,",
"so you can use '2 -1+' or '2 1-' to subtract 1 from 2.  This makes it easier",
"to enter negative numbers and values like '1e-2'.  Any time a token can be",
"interpreted as a numeric value, it is.  This sometimes gets confusing.  For",
"example, '3 1--5-' will compute (3-1)-(-5), pushing 7 onto the stack.",
"'4;1-8' will simply push 3 values onto the stack, while '4;1- 8' will",
"subtract 1 from 4, leaving 3 and 8 on the stack.",
"",
"';' is used to separate entries.  So '1;2+' computes the sum of 1 and 2",
"'_' is an ignored place holder, so 65536 can be written 65_536 or 6_5_53_6",
"',', also used as a place holder.",
"",
"All computation is done using double precision floats, but you can",
"control the output format with k or F.  Although values are parsed",
"using strtod, you must use X and P verses x and p.",
"eg, 0X34P3 will be parsed as the hex value 34 shifted left 3 bits,",
"but 0x34p3 will place 0 on the stack, try to execute the ",
"string in register 0, push decimal 34 on the stack, print it,",
"and then push 3 onto the stack.",
"",
"!    use function from specified register",
"\\f   apply function f to the top value(s) in the stack (eg 0\\sin)",
"D    delete the first register",
"F    use value from the specified register as format string",
"[s]  push s onto the register stack",
"h    print this help message",
"k    set precision of format string",
"m    push value from the specified memory location",
"M    show the memory stack",
"n    print and pop top value of stack",
"p    print top value of stack",
"q    quit",
"r    swap top two elements of the stack",
"R    swap top two elements of the register stack",
"x    execute a string in register",
"y    duplicate top value of stack",
"Y    list (examine) elements of the stack",
"Z    list elements of register stack",
"?    show the current setting of the output format string",
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
#define numeric_tok "+-0123456789XPEabcdef."
#define string_ops "[]D!FRxZ\\"
#define binary_ops "*-+/^r"
#define unary_ops "kny"
#define nonary_ops "hmMpqY?"
#define ignore_char "_,"
#define token_div " \t\n;"

/* We construct the hash table to avoid collisions.  If
a collision happens (this is a compile-time issue), either implement
probing or change the hash function.  For now, we just set the
offset of 3 because 0, 1, and 2 collide.  We could iterate at run
time to find an offset that works, but a better approach is to just
fix as needed when/if we change the set of known functions.
*/
#define HASH_TABLE_SIZE 256
#define HASH_OFFSET 3

struct func;
struct state {
	struct stack *values;
	struct stack *registers;
	struct stack *memory;
	char fmt[32];
	int enquote;
	int escape;
	struct ring_buf *raw;   /* raw input as entered */
	struct ring_buf *accum; /* accumulator (used to re-process) */
	enum { rational, integer } type;
	struct func *function_lut[HASH_TABLE_SIZE];
};

static long double *wrap_get(struct stack *, int);
static int wrap_pop(struct stack *, void *);
static long double sum(struct state *);
struct func {
	const char *name;
	int arg_count;
	union {
		long double (*f)(long double);
		long double (*g)(long double, long double);
		long double (*s)(struct state *);
	};
} functions[] = {
	{ "acos", 1, acosl },
	{ "acosh", 1, acoshl },
	{ "asin", 1, asinl },
	{ "asinh", 1, asinhl },
	{ "atan", 1, atanl },
	{ "atanh", 1, atanhl },
	{ "cbrt", 1, cbrtl },
	{ "cos", 1, cosl },
	{ "cosh", 1, coshl },
	{ "exp", 1, expl },
	{ "exp2", 1, exp2l },
	{ "fabs", 1, fabsl },
	{ "hypot", 2, .g = hypotl },
	{ "lgamma", 1, lgammal },
	{ "log", 1, logl },
	{ "log10", 1, log10l },
	{ "log1p", 1, log1pl },
	{ "log2", 1, log2l },
	{ "nextafter", 2, .g = nextafterl },
	{ "pow", 2, .g = powl},
	{ "sin", 1, sinl },
	{ "sinh", 1, sinhl },
	{ "sum", 0, .s = sum},
	{ "sqrt", 1, sqrtl },
	{ "tan", 1, tanl },
	{ "tgamma", 1, tgammal },
	{ "tanh", 1, tanhl },
	{ NULL, 0 },
};

static void show_functions(void);
static void execute_function(struct state *S, const char *cmd);

void
print_help(struct state *S)
{
	for( const char **s = help; *s; s++ ){
		puts(*s);
	}
	puts("\nThe ! and \\ commands understand the following functions:");
	show_functions();
	putchar('\n');
}

typedef void (*operator) (struct state *, unsigned char, int);
operator char_lut[256];
static void apply_binary(struct state *S, unsigned char c, int);
static void apply_unary(struct state *S, unsigned char c, int);
static void apply_nonary(struct state *S, unsigned char c, int);
static void throw_warning(struct state *S, unsigned char c, int);

void push_it(struct state *, int);

void apply_string_op(struct state *S, unsigned char c);
void die(const char *msg);
void write_args_to_stdin(char *const*argv);
static int push_value(struct state *, unsigned char);
struct ring_buf * select_register(struct state *S);
static void print_stack(struct state *S, struct stack *);
static int get_index(struct state *S);


static size_t
compute_hash(const char *s)
{
	unsigned long rv = 0;
	int p_pow = 1;
	int p = 31;
	int m = HASH_TABLE_SIZE;
	for( ; *s; s += 1 ){
		rv = (rv + (HASH_OFFSET + *s) * p_pow) % m;
		p_pow = (p_pow * p) % m;
	}
	return (size_t)(rv % m);
}


static void
insert_into_lut(struct func *f, struct state *S)
{
	size_t idx = compute_hash(f->name);
	if( S->function_lut[idx] ){
		fprintf(stderr, "Hash collision.  Aborting\n");
		exit(1);
	}
	S->function_lut[idx] = f;
}

static void
hash_functions(struct state *S)
{
	memset(S->function_lut, 0, sizeof S->function_lut);
	struct func *func = functions;
	while(func->name) {
		insert_into_lut(func, S);
		func += 1;
	}
}


static void
init_state(struct state *S)
{
	S->raw = rb_create(32);
	S->accum = rb_create(32);
	S->enquote = 0;
	S->escape = 0;
	S->type = rational;
	S->values = stack_xcreate(sizeof(long double));
	S->memory = stack_xcreate(sizeof(long double));
	S->registers = stack_xcreate(0);
	strcpy(S->fmt, DEFAULT_FMT);
	hash_functions(S);
}

#ifndef BUILD_LUT
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
	stack_free(S->values);
	stack_free(S->memory);
	stack_free(S->registers);
	return 0;
}
#endif


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
		unsigned char c = (unsigned char)k;
		struct ring_buf *b = S->accum;
		int flag;

		if( S->enquote && c != ']' ){
			rb_push(b, c);
		} else if( S->escape && ! strchr(token_div, c)) {
			rb_push(b, c);
		} else if( strchr(numeric_tok, c) ){
			rb_push(b, c);
		} else if( strchr(string_ops, c) ){
			apply_string_op(S, c);
		} else if( strchr(ignore_char, c) ){
			;
		} else {
			flag = push_value(S, c);
			operator f = char_lut[c];
			if( f ){
				f(S, c, flag);
			}
		}
	}
}


static void
push_memory_to_stack(struct state *S)
{
	int offset = get_index(S);
	long double *val;
	if( (val = wrap_get(S->memory, offset)) != NULL ){
		stack_push(S->values, val);
	}
}


static void
show_value(struct state *S, long double val)
{

	if( S->type == rational ){
		printf(S->fmt, val);
	} else {
		long lval = (long)val;
		printf(S->fmt, lval);
	}
}


static void
apply_nonary(struct state *S, unsigned char c, int flag)
{
	long double *val;
	if( flag ){
		return;
	}
	switch( c ){
	default: assert(0);
	case 'Y':
		print_stack(S, S->values);
		break;
	case 'm':
		push_memory_to_stack(S);
		break;
	case 'M':
		print_stack(S, S->memory);
		break;
	case 'p':
		val = wrap_get(S->values, -1);
		if( val ){
			show_value(S, *val);
		}
		break;
	case 'q': exit(0);
	case 'h': print_help(S); /* Fall Thru */
	case '?': printf("Output format currently: %s", S->fmt);
	}
}


/*
 * Collect inputs in the ring buffer and push values onto
 * the stack.  If we encounter an unexpected '-' or '+',
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
	long double val;
	int i;

	cp = start = s;
	if( rb_isempty(b) ){
		return 0;
	}

	while( (i = rb_pop(b)) != EOF ){
		assert( strchr(numeric_tok, i) || S->escape );
		if( start < end ){
			*start++ = i;
		}
	}
	if( S->escape ){
		S->escape = 0;
		if( start + 1 < end ){
			*start = 0;
			execute_function(S, s);
		} else {
			fputs("Overflow: function ignored", stderr);
		}
		return 0;
	}
	if( start == end ){
		fprintf(stderr, "Overflow: Term truncated\n");
		return 0;
	}
	start = s;

	val = strtold(start, &cp);
	while( *cp && strchr("+-", *cp) && cp != start ){
		stack_push(S->values, &val);
		start = cp;
		val = strtold(start, &cp);
	}
	if( *cp && strchr("+-", *cp) ){
		assert( cp == start );
		apply_binary(S, *cp, 0);
		for( char *t = cp + 1; *t; t++ ){
			push_it(S, *t);
		}
		push_it(S, c);
		return 1;
	}
	if( *cp ){
		fprintf(stderr, "Garbled (discarded): %s\n", s);
	} else {
		stack_push(S->values, &val);
	}
	return 0;
}


static void
show_functions(void)
{
	struct func *func = functions;
	int columns = 0;
	putchar('\t');
	while( func->name ){
		columns += printf("%s", func->name);
		func += 1;
		if( func->name ){
			putchar(',');
			putchar(columns > 40 ? '\n' : ' ');
			if( columns > 40 ){
				putchar('\t');
				columns = 0;
			}
		}
	}
	putchar('\n');
}

static int
pop_value(struct state *S, long double *value, int msg)
{
	int (*popper)(struct stack *, void *);

	popper = msg ? wrap_pop : stack_pop;
	int rv = popper(S->values, value);
	if( rv ) {
		stack_push(S->memory, value);
	}
	return rv;
}

static long double *
wrap_get(struct stack *s, int idx)
{
	long double *value = stack_get(s, idx);
	if( value == NULL ){
		fputs("Stack empty\n", stderr);
	}
	return value;
}

static int
wrap_pop(struct stack *s, void *value)
{
	int rv = stack_pop(s, value);
	if( !rv ){
		fputs("Stack empty\n", stderr);
	}
	return rv;
}


static void
execute_function(struct state *S, const char *cmd)
{
	long double arg;
	long double res;
	struct func *func;
	size_t idx;

	idx = compute_hash(cmd);
	assert(idx < sizeof S->function_lut / sizeof *S->function_lut);
	func = S->function_lut[idx];

	if (func && strcmp(cmd, func->name) == 0) {
		switch(func->arg_count){
		default: assert(0);
		case 0:
			res = func->s(S);
			break;
		case 2:
			pop_value(S, &res, 1);
			pop_value(S, &arg, 1);
			res = func->g(arg, res);
			break;
		case 1:
			pop_value(S, &res, 1);
			res = func->f(res);
		}
		stack_push(S->values, &res);
	} else {
		fprintf(stderr, "Unknown function: %s\n", cmd);
	}
}


static void
apply_function(struct state *S, struct ring_buf *rb)
{
	if( !rb ) {
		rb = select_register(S);
	}
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
				"(eg '\%.2Lf' or '\%3Lu')\n", stderr);
		}
	}
}


static int
get_index(struct state *S)
{
	long double val = -1.0;
	int offset = -1;
	if( pop_value(S, &val, 1) ){
		offset = val;
	}
	if( rint(val) != val ){
		offset = -1;
		stack_push(S->values, &val);
	}
	return offset;
}


struct ring_buf *
select_register(struct state *S)
{
	struct ring_buf *ret = NULL;
	int offset = get_index(S);

	if( (ret = stack_get(S->registers, offset)) == NULL ){
		if( offset != -1 ){
			fprintf(stderr, "Register %d empty\n", offset);
		}
	}
	return ret;
}

void
apply_string_op(struct state *S, unsigned char c)
{
	struct ring_buf *a = NULL, *rb = NULL;
	void *e;
	assert( !S->enquote || c == ']' );
	if( c != ']' ){
		push_value(S, c);
	}
	switch( c ){
	case '\\':
		S->escape = 1;
		break;
	case '[':
		S->enquote = 1;
		break;
	case ']':
		S->enquote = 0;
		stack_xpush(S->registers, S->accum);
		S->accum = rb_create(32);
		break;
	case '!':
		if( (rb = select_register(S)) != NULL ){
			apply_function(S, rb);
		}
		break;
	case 'D':
		wrap_pop(S->registers, &e);
		break;
	case 'F':
		extract_format(S);
		break;
	case 'R':
		if( wrap_pop(S->registers, &a) &&
			wrap_pop(S->registers, &rb) ){
			stack_push(S->registers, a);
			stack_push(S->registers, rb);
		}
		break;
	case 'Z':
		for( unsigned i = 0; i < stack_size(S->registers); i++ ){
			int j = 0, c;
			rb = stack_get(S->registers, i);

			printf("(%d): ", i);
			while( (c = rb_peek(rb, j++)) != EOF ){
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

static void
print_stack(struct state *S, struct stack *v)
{
	unsigned i = 0;
	long double *s;
	for( s = stack_base(v); i < stack_size(v); s++, i++ ){
		printf("%3u: ", i);
		printf(S->fmt, *s);
	}
}


static void
apply_unary(struct state *S, unsigned char c, int flag)
{
	long double val;
	assert( strchr(unary_ops, c) );
	if( flag || !pop_value(S, &val, 1) ){
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
	case 'n':
		show_value(S, val);
		break;
	}
}

static void
apply_binary(struct state *S, unsigned char c, int flag)
{
	if( flag ){
		return;
	}
	long double val[2];
	long double res;
	assert( strchr(binary_ops, c));
	if( !pop_value(S, val, 1) || !pop_value(S, val + 1, 1) ){
		return;
	}
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

static long double
sum(struct state *S)
{
	long double value;
	long double sum = 0.0;
	int rv;

	while( (rv = pop_value(S, &value, 0)) != 0 ){
		sum += value;
	}

	return sum;
}

#ifdef BUILD_LUT
/* Use this and copy-paste into calc.c */
int
main(void)
{
	puts("operator char_lut[256] = {");
	fputs("throw_warning,", stdout);
	for( unsigned c = 1; c < 256; c += 1 ){
		char *msg = "throw_warning";
		if( strchr(binary_ops, c)) {
			msg = "apply_binary";
		} else if( strchr(unary_ops, c) ){
			msg = "apply_unary";
		} else if( strchr(nonary_ops, c) ){
			msg = "apply_nonary";
		} else if( strchr(token_div, c) ){
			msg = "NULL";
		}
		putchar( c % 5 ? ' ' : '\n');
		fputs(msg, stdout);
		if( c != 255 ){
			putchar(',');
		}
	}
	puts("};");
}
#endif

static void
throw_warning(struct state *S, unsigned char c, int f)
{
	fprintf( stderr, "Unexpected: %c\n", c );
}

operator char_lut[256] = {
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, NULL,
NULL, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, NULL, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, apply_binary, apply_binary, throw_warning,
apply_binary, throw_warning, apply_binary, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, NULL,
throw_warning, throw_warning, throw_warning, apply_nonary, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, apply_nonary, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, apply_nonary,
throw_warning, throw_warning, throw_warning, throw_warning, apply_binary,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, apply_nonary,
throw_warning, throw_warning, apply_unary, throw_warning, apply_nonary,
apply_unary, throw_warning, apply_nonary, apply_nonary, apply_binary,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, apply_unary, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning, throw_warning, throw_warning, throw_warning, throw_warning,
throw_warning};
