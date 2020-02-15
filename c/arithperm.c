/*
 * Perform permutations of arithmetic on a fixed set of values.
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
	int count;        /* Number of operands */
	struct element {
		double val;
		char descr[512];  /* Human readable description (for infix) */
	} *stack;                 /* Stack used for computation */
};

void * xmalloc(size_t s);
void parse_cmd_line(int argc, char **argv, struct operation*);
int next_op(struct operation *op);
void render(struct operation *op);

int
main(int argc, char **argv)
{
	struct operation op;

	parse_cmd_line(argc, argv, &op);
	while(next_op(&op)) {
		render(&op);
	}
	free(op.operands);
	free(op.stack);
}


void
render(struct operation *op)
{
	int c = 0;
	uint32_t m = op->mask;
	char *ops = op->operators;
	struct element *sp = op->stack;

	while( m ) {
		if( m & 0x1 ) { /* Apply an operator */
			char buf[1024];
			if(sp - op->stack < 2) {
				return; /* underflow indicates invalid mask.  Ignore the error */
			}
			sp -= 1;
			snprintf(buf, sizeof buf, "(%s %c %s)", sp[-1].descr, *ops, sp->descr);
			strncpy(sp[-1].descr, buf, sizeof sp->descr);
			sp[-1].descr[sizeof sp->descr - 1] = '\0';
			switch(*ops) {
			case '+': sp[-1].val += sp->val; break;
			case '-': sp[-1].val -= sp->val; break;
			case '*': sp[-1].val *= sp->val; break;
			case '/': sp[-1].val /= sp->val; break;
			default: assert(0);
			}
			ops += 1;
		} else {
			assert( c < op->count );
			sp->val = op->operands[c++];
			snprintf(sp->descr, sizeof sp->descr, "%g", sp->val);
			sp += 1;
			assert( sp - op->stack <= op->count );
		}
		m >>= 1;
	}
	sp -= 1;
	printf("%s = %g\n", sp->descr, sp->val);
	return;
}


int
mask_is_invalid(uint32_t m)
{
	/* A mask is invalid (will lead to underflow) unless it meets the following conditions:
	 *  The 2 LSB must be 0
	 *  For any given bit, the number of unset bits to the right must of it must be greater
	 *  that the number of set bits.
	 */
	int i, sum;
	for( sum = i = 0; i < 32; i++, m >>= 1) {
		if( m & 0x1) {
			sum += 1;
		} else {
			sum -= 1;
		}
		if( sum >= 0 ) { /* This mask is invalid */
			return 1;
		}
	}
	return 0; /* Not necessarily valid, but this check doesn't notice */
}


/*
 * generate the next mask with N bits set. Mask is 2N+1 wide.
 * Taken from:
 * https://stackoverflow.com/questions/26594951/finding-next-bigger-number-with-same-number-of-set-bits
 */
uint32_t
compute_next_mask(uint32_t x)
{
	uint32_t c = x & -x;
	uint32_t r = x + c;
	return (((r ^ x) >> 2) / c) | r;
}

uint32_t
next_mask(int N, uint32_t x)
{
	assert(N < 15);
	uint32_t max = 1 << (2*N + 1);

	do x = compute_next_mask(x); while( mask_is_invalid(x));

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
	if(strspn( op->operators, "/" ) == (unsigned)op->count - 1) {
		op->mask = next_mask(op->count - 1, op->mask);
	}
	next_perm(op->operators);
	return !!op->mask;
}


/*
 * Initialize struct operation from the command line args.
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
	op->operators = strndup("++++++++++++++++++++", op->count - 1);
	if(op->operators == NULL) {
		err(EXIT_FAILURE, "strndup");
	}
	op->mask = (( 0x1 << ( op->count - 1 )) - 1) << 2;
	op->operands = op->operands;
	op->stack = xmalloc( op->count * sizeof *op->stack);
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
