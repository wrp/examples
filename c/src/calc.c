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
"C    pop and discard the top item in the stack",
"D    pop and discard the top item in the register stack",
"F    use value from the specified register as format string",
"[s]  push s onto the register stack",
"h    print this help message",
"i    set input base (0 for float)",
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
#define string_ops "()[]D!FRxZ\\"
#define binary_ops "*-+/^r"
#define unary_ops "Ckny"
#define nonary_ops "hmMpqY?"
#define ignore_char "_,"

#define HASH_OFFSET 3 /* (hash-table-note) */
#define HASH_TABLE_SIZE 256
/*
 * (hash-table-note) We construct the hash table to avoid collisions.  If
 * a collision happens (this is a compile-time issue), either implement
 * probing or change the hash function.  For now, we just set the
 * offset of 3 because 0, 1, and 2 collide.  We could iterate at run
 * time to find an offset that works, but a better approach is to just
 * fix as needed when/if we change the set of known functions.
 */

enum number_type { rational, integer };
struct func;

struct format_string {
	char fmt[32];
	char *specifier;  /* location of 'd' or 'g', etc. in fmt. */
	char integer_specifier;  /* 'd', or 'i', etc. */
	char float_specifier;    /* 'g', 'e', etc. */
};
struct state;
typedef void (*process)(struct state *, int);
static void process_normal(struct state *S, int c);
static void process_enquote(struct state *S, int c);
static void process_paren(struct state *S, int c);
static void process_escape(struct state *S, int c);

struct state {
	struct stack *values;
	struct stack *registers;
	struct stack *memory;
	struct format_string fmt;
	process processor;

	int input_base;
	struct ring_buf *raw;   /* raw input as entered */
	struct ring_buf *accum; /* accumulator (used to re-process) */
	enum number_type type;
	struct func *function_lut[HASH_TABLE_SIZE];
};
struct stack_entry {
	union {
		long double lf;
		long long ld;
	} v;
	enum number_type type;
	int stored;  /* bool */
};

static int pop_value(struct state *, struct stack_entry *, int);
static struct stack_entry *wrap_get(struct stack *, int);
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

void push_raw(struct state *, int);

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
	S->processor = process_normal;
	S->type = rational;
	S->input_base = 0;
	S->values = stack_xcreate(sizeof(struct stack_entry));
	S->memory = stack_xcreate(sizeof(struct stack_entry));
	S->registers = stack_xcreate(0);
	strcpy(S->fmt.fmt, DEFAULT_FMT);
	hash_functions(S);
}

#ifndef BUILD_LUT
int
main(int argc, char **argv)
{
	int c;
	struct state S[1];

	assert( char_lut['d'] == throw_warning);
	assert( char_lut['C'] == apply_nonary);
	setlocale(LC_NUMERIC, "");
	init_state(S);

	if( argc > 1 ){
		for( ; *++argv; push_raw(S, ' ') ){
			for( char *t = *argv; *t; t++ ){
				push_raw(S, *t);
			}
		}
	} else while( (c=getchar()) != EOF ){
		push_raw(S, c);
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
 * Push an item onto the raw buffer and then read it until empty.  Some
 * operations (notably 'x') will push additional values into the ring buffer,
 * and they should be processed before the next entry coming from input.
 */
void
push_raw(struct state *S, int c)
{
	rb_push(S->raw, (unsigned char)c);
	while( (c = rb_pop( S->raw )) != EOF ){
		S->processor(S, c);
	}
}


static void
process_escape(struct state *S, int c)
{
	if( ! isspace(c) ){
		rb_push(S->accum, c);
	} else {
		apply_string_op(S, c);
	}
}

static void
process_paren(struct state *S, int c)
{
	if( c == ')' ){
		apply_string_op(S, c);
	} else {
		rb_push(S->accum, c);
	}
}

static void
process_enquote(struct state *S, int c)
{
	if( c != ']' ){
		rb_push(S->accum, c);
	}  else {
		apply_string_op(S, c);
	}
}

static void
process_normal(struct state *S, int c)
{
	struct ring_buf *b = S->accum;

	if( strchr(numeric_tok, c) ){
		rb_push(b, c);
	} else if( strchr(string_ops, c) ){
		apply_string_op(S, c);
	} else if( strchr(ignore_char, c) ){
		;
	} else {
		int flag = push_value(S, c);
		operator f = char_lut[c];
		if( f ){
			f(S, c, flag);
		}
	}
}


static void
push_memory_to_stack(struct state *S)
{
	int offset = get_index(S);
	struct stack_entry *val;
	if( (val = wrap_get(S->memory, offset)) != NULL ){
		assert(val->stored == 1);
		stack_push(S->values, val);
	}
}


static void
show_value(struct state *S, struct stack_entry *val)
{
	if( val->type == rational ){
		printf(S->fmt.fmt, val->v.lf);
	} else {
		printf(S->fmt.fmt, (long)val->v.lf);
	}
}


static void
apply_nonary(struct state *S, unsigned char c, int flag)
{
	struct stack_entry *val, v;
	if( flag ){
		return;
	}
	switch( c ){
	default: assert(0);
	case 'C':
		pop_value(S, &v, 1);
		break;
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
			show_value(S, val);
		}
		break;
	case 'q': exit(0);
	case 'h': print_help(S); /* Fall Thru */
	case '?': printf("Output format currently: %s", S->fmt.fmt);
	}
}


static void
read_val(struct state *S, struct stack_entry *v, char *s, char **cp)
{
	v->v.lf = strtold(s, cp);
	v->type = S->type;
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
	char start[256] = "", *end = start + sizeof start;
	char *cp, *s;
	struct stack_entry val = { 0 };
	int i;

	cp = s = start;
	if( rb_isempty(b) ){
		return 0;
	}

	while( (i = rb_pop(b)) != EOF ){
		assert( strchr(numeric_tok, i) ||
			(S->processor == process_escape) );
		if( s < end ){
			*s++ = i;
		}
	}

	if( s < end ){
		*s++ = '\0';
	} else {
		fprintf(stderr, "Overflow: Term ignored\n");
		return 0;
	}

	if( S->processor == process_escape ){
		S->processor = process_normal;
		execute_function(S, start);
		return 0;
	}
	s = start;

	read_val(S, &val, s, &cp);
	while( *cp && strchr("+-", *cp) && cp != s ){
		stack_push(S->values, &val);
		s = cp;
		read_val(S, &val, s, &cp);
	}
	if( *cp && strchr("+-", *cp) ){
		assert( cp == s );
		apply_binary(S, *cp, 0);
		for( char *t = cp + 1; *t; t++ ){
			push_raw(S, *t);
		}
		push_raw(S, c);
		return 1;
	}
	if( *cp ){
		fprintf(stderr, "Garbled (discarded): %s\n", start);
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
pop_value(struct state *S, struct stack_entry *value, int msg)
{
	int (*popper)(struct stack *, void *);

	popper = msg ? wrap_pop : stack_pop;
	int rv = popper(S->values, value);
	if( rv && !value->stored ) {
		value->stored = 1;
		stack_push(S->memory, value);
	}
	return rv;
}

static struct stack_entry *
wrap_get(struct stack *s, int idx)
{
	struct stack_entry *value = stack_get(s, idx);
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
	struct stack_entry arg;
	struct stack_entry res;
	struct func *func;
	size_t idx;

	idx = compute_hash(cmd);
	assert(idx < sizeof S->function_lut / sizeof *S->function_lut);
	func = S->function_lut[idx];

	if (func && strcmp(cmd, func->name) == 0) {
		switch(func->arg_count){
		default: assert(0);
		case 0:
			res.v.lf = func->s(S);
			break;
		case 2:
			pop_value(S, &res, 1);
			pop_value(S, &arg, 1);
			res.v.lf = func->g(arg.v.lf, res.v.lf);
			break;
		case 1:
			pop_value(S, &res, 1);
			res.v.lf = func->f(res.v.lf);
		}
		res.stored = 0;
		res.type = rational;
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
		char *b = S->fmt.fmt, *e = S->fmt.fmt + sizeof S->fmt.fmt;
		int count = 0;
		int c = 0;

		while( b < e && (c = rb_peek(rb, b - S->fmt.fmt)) != EOF ){
			/* Extremely naive check of format string.  */
			if( count == 2 ){
				if( strchr("diouxX", c) ){
					S->fmt.specifier = b;
					S->type = integer;
				} else if( strchr("fega", c) ){
					S->fmt.specifier = b;
					S->type = rational;
				}
			}
			*b++ = c;
			count += !count && c == '%';
			count += count && c == 'L';
		}
		if( b > S->fmt.fmt && b + 1 < e && b[-1] != '\n' ){
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
	struct stack_entry val = { .v.lf = -1.0 };
	int offset = -1;
	if( pop_value(S, &val, 1) ){
		offset = val.v.lf;
	}
	if( rint(val.v.lf) != val.v.lf ){
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
	assert( (S->processor != process_enquote) || (c == ']') );
	if( c != ']' ){
		push_value(S, c);
	}
	switch( c ){
	case '\\':
		S->processor = process_escape;
		break;
	case '(':
		S->processor = process_paren;
		break;
	case ')':
		S->processor = process_normal;
		rb_push(S->raw, ' ');
		break;
	case '[':
		S->processor = process_enquote;
		break;
	case ']':
		S->processor = process_normal;
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
	struct stack_entry *s;
	for( s = stack_base(v); i < stack_size(v); s++, i++ ){
		printf("%3u: ", i);
		printf(S->fmt.fmt, s->v);
	}
}


static void
apply_unary(struct state *S, unsigned char c, int flag)
{
	struct stack_entry val;
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
		if( val.v.lf < 1 ){
			snprintf(S->fmt.fmt, sizeof S->fmt.fmt, "%%'Ld\n");
			S->type = integer;
		} else {
			snprintf(S->fmt.fmt, sizeof S->fmt.fmt, "%%.%dLf\n",
				(int)val.v.lf);
			S->type = rational;
		}
		break;
	case 'n':
		show_value(S, &val);
		break;
	}
}

static void
apply_binary(struct state *S, unsigned char c, int flag)
{
	if( flag ){
		return;
	}
	struct stack_entry val[2];
	struct stack_entry res = {0};
	assert( strchr(binary_ops, c));
	if( !pop_value(S, val, 1) || !pop_value(S, val + 1, 1) ){
		return;
	}
	switch(c) {
	case 'r':
		stack_push(S->values, val);
		res = val[1];
		break;
	case '*': res.v.lf = val[1].v.lf * val[0].v.lf; break;
	case '-': res.v.lf = val[1].v.lf - val[0].v.lf; break;
	case '+': res.v.lf = val[1].v.lf + val[0].v.lf; break;
	case '/': res.v.lf = val[1].v.lf / val[0].v.lf; break;
	case '^': res.v.lf = pow(val[1].v.lf, val[0].v.lf); break;
	}
	stack_push(S->values, &res);
}

static long double
sum(struct state *S)
{
	struct stack_entry value;
	struct stack_entry sum = {0};
	int rv;

	while( (rv = pop_value(S, &value, 0)) != 0 ){
		sum.v.lf += value.v.lf;
	}

	return sum.v.lf;
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
		} else if( isspace(c) ){
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
throw_warning, throw_warning, apply_nonary, throw_warning, throw_warning,
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
