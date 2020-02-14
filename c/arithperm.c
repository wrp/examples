/* Read N numbers.  Perform permutations of arithmetic on them.
 * Inspired by https://stackoverflow.com/questions/60196706/is-there-a-way-to-interchange-mathematical-operators-in-a-while-or-for-loop
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <assert.h>

struct operation {
	double *operands; /* The numbers to be manipulated */
	char *operators;  /* operators to apply, eg "++/-*" */
	uint32_t mask;    /* A bit mask showing where to apply operators */
	int count;     /* Number of operands */
};

void * xmalloc(size_t s);
void parse_cmd_line(int argc, char **argv, struct operation*);
int next_op(struct operation *op);
int render(struct operation *op);

int
main(int argc, char **argv)
{
	struct operation op;

	parse_cmd_line(argc, argv, &op);

	while(next_op(&op)) {
		render(&op);
	}
}

struct element {
	double val;
	char descr[512];
};

int
render(struct operation *op)
{
	int c = 0;
	uint32_t m = op->mask;
	char *ops = op->operators;
	struct element *stack = xmalloc( op->count * sizeof *stack);
	struct element *sp = stack;

	while( m || c < op->count ) {
		if( m & 0x1 ) { /* Apply an operator */
			char buf[1024];
			if(sp - stack < 2) {
				return 1;
			}
			sp -= 1;
			snprintf(buf, sizeof buf, "(%s %c %s)", sp[-1].descr, *ops, sp->descr);
			strncpy(sp[-1].descr, buf, sizeof sp->descr);
			switch(*ops) {
			case '+': sp[-1].val += sp[0].val; break;
			case '-': sp[-1].val -= sp[0].val; break;
			case '*': sp[-1].val *= sp[0].val; break;
			case '/': sp[-1].val /= sp[0].val; break;
			default: assert(0);
			}
			ops += 1;
		} else {
			if( c >= op->count ) {
				return 1;
			}
			sp->val = op->operands[c++];
			snprintf(sp->descr, sizeof sp->descr, "%g", sp->val);
			sp += 1;
		}
		m >>= 1;
	}

	printf("%s = %g\n", sp[-1].descr, sp[-1].val);
	return 0;
}


/* generate the next uint32_t with N bits set  */
uint32_t
next_mask(int N)
{
	static uint32_t x = 0;
	uint32_t max = 1 << (2*N + 1);

	assert(N < 15);
	while( x < max &&  __builtin_popcount(++x) != N)
		;
	return x < max ? x : 0;
}

/* Given a string of operators (eg "+*-/++"), generate the next permutation */
void
next_perm( char *s )
{
	size_t len = strlen(s);
	assert(strspn( s, "+-*/" ) == len);
	for( int i = 0; i < (int)len; i++) {
		switch(s[i]) {
		case '+': s[i] = '-'; return;
		case '-': s[i] = '*'; return;
		case '*': s[i] = '/'; return;
		case '/': s[i] = '+'; break;
		}
	}
}

int
next_op(struct operation *op)
{
	static struct operation *b = NULL;

	if( b == NULL ) {
		b = xmalloc( sizeof *b );
		b->operators = strndup("++++++++++++++++++++", op->count - 1);
		if(b->operators == NULL) {
			err(EXIT_FAILURE, "strndup");
		}
		b->mask = next_mask(op->count - 1);
		b->operands = op->operands;
		b->count = op->count;
	} else {
		if(strspn( b->operators, "/" ) == (unsigned)b->count - 1) {
			b->mask = next_mask(op->count - 1);
		}
		next_perm(b->operators);
	}
	if( b->mask == 0)
		return 0;
	memcpy(op, b, sizeof *op);
	return 1;
}


/*
 * Create an array of doubles consisting of all the command line arguments
 * Caller should free.
 */
void
parse_cmd_line(int argc, char **argv, struct operation *op)
{
	double *v;
	char *defaults[] = { argv[0], "2.0", "1.0", NULL };
	if(argc < 3) {
		argc = 3;
		if(argv[1] != NULL)
			defaults[1] = argv[1];
		argv = defaults;
	}

	argv += 1;
	argc -= 1;
	v = xmalloc( sizeof *v * argc);

	for( ;*argv; argv++, v++ ) {
		char *end;
		*v = strtod(*argv, &end);
		if(*end != '\0') {
			errx(EXIT_FAILURE, "Invalid input in \"%s\""
				"at position %ld.  Unexpected value: '%c'",
				*argv, end - *argv, *end);
		}
	}
	assert(*argv == NULL);
	op->operands = v - argc;
	op->count = argc;
}


void *
xmalloc(size_t s)
{
	void *r = malloc(s);
	if(r == NULL) {
		err(EXIT_FAILURE, "malloc");
	}
	return r;
}
