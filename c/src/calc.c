const char *help[] = {
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
"#    set input base (0 for float)",
"\\f   apply function f to the top value(s) in the stack (eg 0\\sin)",
"C    pop and discard the top item in the stack",
"D    pop and discard the top item in the register stack",
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

#define numeric_tok "+-0123456789." \
	"abcdefghijklmnopqrstuvwxyz" \
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define string_ops "()[]D!FRxZ\\"
#define binary_ops "*-+/^r"
#define unary_ops "C#kny"
#define nonary_ops "hmMpqY?"
#define ignore_char "_,"

#define DEFAULT_FORMAT ".10'"
#define DEFAULT_INT_FORMAT "'"

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

struct state;
typedef void (*process)(struct state *, int);
static void process_normal(struct state *S, int c);
static void process_enquote(struct state *S, int c);
static void process_paren(struct state *S, int c);
static void process_escape(struct state *S, int c);
typedef void (*operator) (struct state *, unsigned char);
operator operator_lut[256];
static void build_lut(struct state *);

struct state {
	struct stack *values;
	struct stack *registers;
	struct stack *memory;
	char format[2][32];
	char specifier;
	process processor;

	int input_base;
	struct ring_buf *raw;   /* raw input as entered */
	struct ring_buf *accum; /* accumulator (used to re-process) */
	struct func *function_lut[HASH_TABLE_SIZE];
	int plus_minus_count;
};
union value {
	long double lf;
	long long ld;
};
struct stack_entry {
	union value v;
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

static void apply_binary(struct state *S, unsigned char c);
static void apply_unary(struct state *S, unsigned char c);
static void apply_nonary(struct state *S, unsigned char c);
static void throw_warning(struct state *S, unsigned char c);

void push_raw(struct state *, int);

void apply_string_op(struct state *S, unsigned char c);
void die(const char *msg);
void write_args_to_stdin(char *const*argv);
static int push_value(struct state *);
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
	S->input_base = 0;
	S->specifier = 'g';
	strncpy(S->format[0], DEFAULT_FORMAT, sizeof S->format[0]);
	strncpy(S->format[1], DEFAULT_INT_FORMAT, sizeof S->format[1]);
	S->values = stack_xcreate(sizeof(struct stack_entry));
	S->memory = stack_xcreate(sizeof(struct stack_entry));
	S->registers = stack_xcreate(0);
	S->plus_minus_count = 0;
	hash_functions(S);
	build_lut(S);
}

int
main(int argc, char **argv)
{
	int c;
	struct state S[1];

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


/*
 * Push an item onto the raw buffer and then read it until empty.  Some
 * operations (notably 'x') will push additional values into the ring buffer,
 * and they should be processed before the next entry coming from input.
 */
void
push_raw(struct state *S, int c)
{
	rb_xpush(S->raw, (unsigned char)c);
	while( (c = rb_pop( S->raw )) != EOF ){
		S->processor(S, c);
	}
}


static int
get_term(struct state *S, char *buf, size_t siz)
{
	char *s = buf, *end = buf + siz;
	int i;

	if( rb_isempty(S->accum) ){
		return 0;
	}

	while( (i = rb_pop(S->accum)) != EOF ){
		if( s < end ){
			*s++ = i;
		}
	}
	if( s < end ){
		*s = '\0';
		return 1;
	} else {
		fprintf(stderr, "Overflow: Term ignored\n");
		return 0;
	}
}

static void
normalize_state(struct state *S)
{
	S->plus_minus_count = 0;
	S->processor = process_normal;
}

static void
process_escape(struct state *S, int c)
{
	if( isspace(c) ){
		int i;
		char buf[256];

		normalize_state(S);
		if( get_term(S, buf, sizeof buf) ){
			execute_function(S, buf);
		}
	} else {
		rb_xpush(S->accum, c);
	}
}

static void
process_paren(struct state *S, int c)
{
	if( c == ')' ){
		normalize_state(S);
		push_value(S);
	} else {
		rb_xpush(S->accum, c);
	}
}

static void
process_enquote(struct state *S, int c)
{
	if( c != ']' ){
		rb_xpush(S->accum, c);
	} else {
		normalize_state(S);
		stack_xpush(S->registers, S->accum);
		S->accum = rb_create(32);
	}
}

static void
process_normal(struct state *S, int c)
{
	struct ring_buf *b = S->accum;

	if( (c == '+') || (c == '-') ){
		S->plus_minus_count += 1;
		if( S->plus_minus_count > 3 ){
			S->plus_minus_count = 0;
			push_value(S);
		}
	}

	if( strchr(numeric_tok, c) ){
		rb_xpush(b, c);
	} else if( strchr(ignore_char, c) ){
		;
	} else {
		/*
		 * This symbol looks like it is potentially a boundary on
		 * a number, so try to parse the accumulator.  But there
		 * may be some cruft in the accumulator that is not part
		 * of a number.  Eg given input "+4e-3+++n", we won't get
		 * here until we see the 'n'. push_value() will push those
		 * back on the raw buffer, and we push this symbol on
		 * *after* those symbols to let it get processed in the
		 * proper order.
		 */
		if( push_value(S) ){
			push_raw(S, c);
		} else {
			operator f = operator_lut[c];
			if( f ){
				f(S, c);
			}
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
		stack_xpush(S->values, val);
	}
}


static void
show_value(struct state *S, struct stack_entry *val)
{
	char fmt[64] = "%";
	if( val->type == rational ){
		snprintf(fmt + 1, 63, "%sL%c", S->format[0], S->specifier);
		printf(fmt, val->v.lf);
	} else {
		snprintf(fmt + 1, 63, "%slld", S->format[1]);
		printf(fmt, val->v.ld);
	}
	putchar('\n');
}


static void
apply_nonary(struct state *S, unsigned char c)
{
	struct stack_entry *val, v;
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
	case '?': printf("Output formats rational:%s%c  integer: %sd",
		S->format[0], S->specifier, S->format[1]);
	}
}


static void
read_val(struct state *S, struct stack_entry *v, char *s, char **end)
{
	if( S->input_base ){
		v->v.ld = strtoll(s, end, S->input_base);
		v->type = integer;
	} else {
		v->v.lf = strtold(s, end);
		v->type = rational;
	}
}

static int
is_operator(char c)
{
	operator *f = operator_lut;
	return c && f[c] && f[c] != throw_warning;
}


/*
 * Collect inputs in the ring buffer and push values onto
 * the stack.  If we encounter an unexpected '-' or '+',
 * treat it as a binary operator and push the rest of
 * the buffer back on the stack to be processed, and
 * return non-zero to indicate that has happened.
 */
int
push_value(struct state *S)
{
	char buf[256];
	char *cp, *s;
	struct stack_entry val = { 0 };
	operator *f = operator_lut;
	int i;

	if( !get_term(S, buf, sizeof buf) ){
		return 0;
	}

	cp = s = buf;
	read_val(S, &val, s, &cp);
	while( cp != s && is_operator(*cp) ){
		stack_xpush(S->values, &val);
		s = cp;
		read_val(S, &val, s, &cp);
	}
	if( is_operator(*cp) ){
		assert( cp == s );
		f[*cp](S, *cp);
		for( char *t = cp + 1; *t; t++ ){
			push_raw(S, *t);
		}
		return 1;
	}
	if( *cp ){
		fprintf(stderr, "Garbled Input");
		if( S->input_base ){
			fprintf(stderr, " (input base is %d)", S->input_base);
		}
		fprintf(stderr, ": Discarded '%s'\n", buf);
	} else {
		stack_xpush(S->values, &val);
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
		stack_xpush(S->memory, value);
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
		stack_xpush(S->values, &res);
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
	/* This is completely deprecated.  Just parsing the fmt
	 * string to defer changing the test suite.
	 */
	struct ring_buf *rb = select_register(S);
	if( rb == NULL ){
		return;
	}
	int c = 0;
	int i = 0;
	int fmt_idx = S->input_base != 0;
	char *dest = S->format[fmt_idx];
	char *end = S->format[fmt_idx] + sizeof S->format[0];

	while( dest < end && ((c = rb_peek(rb, i)) != EOF )){
		i += 1;
		if( c == '%' || c == 'L') {
			continue;
		}
		if( strchr("diouxXfega", c) ){
			S->specifier = c;
			continue;
		}
		*dest++ = c;
	}
	if( dest < end ){
		*dest++ = '\0';
	}
}


static int
get_index(struct state *S)
{
	struct stack_entry val = { .v.lf = -1.0 };
	int offset = -1;
	if( pop_value(S, &val, 1) ){
		offset = val.type == rational ? val.v.lf : val.v.ld;
	}
	if( val.type == rational && (rint(val.v.lf) != val.v.lf) ){
		offset = -1;
		stack_xpush(S->values, &val);
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
	assert( c != ']' );
	switch( c ){
	case '\\':
		S->processor = process_escape;
		break;
	case '(':
		S->processor = process_paren;
		break;
	case '[':
		S->processor = process_enquote;
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
			stack_xpush(S->registers, a);
			stack_xpush(S->registers, rb);
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
				rb_xpush(S->raw, c);
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
		show_value(S, s);
	}
}


static void
apply_unary(struct state *S, unsigned char c)
{
	struct stack_entry val;
	int precision;
	assert( strchr(unary_ops, c) );
	if( !pop_value(S, &val, 1) ){
		return;
	}
	enum number_type t = val.type;
	union value v = val.v;
	switch( c ){
	case '#':
		S->input_base = t == rational ? rint(v.lf) : v.ld;
		break;
	case 'y':
		stack_xpush(S->values, &val);
		stack_xpush(S->values, &val);
		break;
	case 'k':
		precision =  t==rational ? rint(v.lf) : v.ld;
		snprintf(S->format[0], sizeof *S->format, ".%d'", precision);
		S->specifier = 'f';
		break;
	case 'n':
		show_value(S, &val);
		break;
	}
}

static void
apply_binary(struct state *S, unsigned char c)
{
	struct stack_entry val[2];
	struct stack_entry res = {0};
	assert( strchr(binary_ops, c));
	if( !pop_value(S, val, 1) || !pop_value(S, val + 1, 1) ){
		return;
	}
	switch(c) {
	case 'r':
		stack_xpush(S->values, val);
		res = val[1];
		break;
	case '*': res.v.lf = val[1].v.lf * val[0].v.lf; break;
	case '-': res.v.lf = val[1].v.lf - val[0].v.lf; break;
	case '+': res.v.lf = val[1].v.lf + val[0].v.lf; break;
	case '/': res.v.lf = val[1].v.lf / val[0].v.lf; break;
	case '^': res.v.lf = pow(val[1].v.lf, val[0].v.lf); break;
	}
	stack_xpush(S->values, &res);
}

static long double
sum(struct state *S)
{
	struct stack_entry value;
	struct stack_entry sum = {0};
	int rv;

	while( (rv = pop_value(S, &value, 0)) != 0 ){
		enum number_type t = value.type;
		union value v = value.v;
		sum.v.lf += t == rational ? v.lf : v.ld;
	}
	assert(sum.type == rational);
	assert(sum.type == 0);

	return sum.v.lf;
}

static void
build_lut(struct state *S)
{
	for( unsigned c = 0; c < 256; c += 1 ){
		operator f = throw_warning;
		if( strchr(binary_ops, c)) {
			f = apply_binary;
		} else if( strchr(unary_ops, c) ){
			f = apply_unary;
		} else if( strchr(nonary_ops, c) ){
			f = apply_nonary;
		} else if( strchr(string_ops, c) ){
			f = apply_string_op;
		} else if( isspace(c) ){
			f = NULL;
		}
		operator_lut[c] = f;
	}
}

static void
throw_warning(struct state *S, unsigned char c)
{
	fprintf( stderr, "Unexpected: %c\n", c );
}
