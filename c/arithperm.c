/*
 * Perform permutations of arithmetic on a fixed set of values.
 * Inspired by https://stackoverflow.com/questions/60196706/is-there-a-way-to-interchange-mathematical-operators-in-a-while-or-for-loop
 *
 * We take a set of double values as parameters and generate all
 * possible arithmetic operations using +, -, *, and /.  The key
 * observation is that with N values, there are N-1 operators.  We
 * generate all possible combintations by building masks that are 2N
 * - 1 wide, where the set bit indicates that an operator is to be
 * applied.  For example, with N = 3, the mask 0x18 would correspond
 * to the grouping ABC** (Things are reversed.  The mask is '11000',
 * and reading from right to left gives 3 operands, follwed by 2
 * operators, in postfix.)  In infix notation, that example would be
 * (A * (B * C)).  For each mask, we iterate over all permutations of
 * +,-,*,/
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <assert.h>

struct expression {
	double *operands;    /* The numbers to be manipulated */
	char operators[16];  /* operators to apply, eg "++/-*" */
	uint32_t mask;       /* A bit mask showing where to apply operators */
	int count;           /* Number of operands */
	struct element {
		double val;
		int last;
		char descr[512];  /* Human readable description (for infix) */
	} *stack;                 /* Stack used for computation */
};

void * xmalloc(size_t s);
void parse_cmd_line(int argc, char **argv, struct expression*);
int next_op(struct expression *);
void eval(struct expression *);

int
main(int argc, char **argv)
{
	struct expression exp;

	parse_cmd_line(argc, argv, &exp);
	do eval(&exp); while( next_op(&exp) );
	free(exp.operands);
	free(exp.stack);
}


void
compute_fmt(struct element *sp, char *fmt, char op, int flag)
{
	char *paren = "(%s)";
	char *noparen = "%s";
	char *template[2] = {noparen, noparen};
	(void)flag;
	switch( op ){
	case '/':
		if( sp->last != '.' ){
			template[0] = paren;
		}
		if( sp[1].last != '.' ){
			template[1] = paren;
		}
		break;
	case '*':
		if( sp->last != '.' && (strchr("+-", sp->last) || sp->last != op) ){
			template[0] = paren;
		}
		if( sp[1].last != '.' && (strchr("+-", sp[1].last) || sp[1].last != op) ){
			template[1] = paren;
		}
		break;
	case '-':
		if( sp[1].last != '.' && strchr("+-", sp[1].last) ){
			template[1] = paren;
		}
		break;
	}
	sprintf(fmt, "%s %%c %s", template[0], template[1]);

	return;
}


/*
 * Evaluate the expression and pretty print it to stdout
 *
 * TODO: rework the mask.  Instead of having a string of operands and
 * a mask that inidcates position, build the string like "00-0*00*+",
 * where the 0 indicates the position of an operand.  This will cleanup
 * the eval a bit, since there would just be a switch and no initial
 * if. And, deal with grouping better.  It feels like we can easily
 * get '1 + 2 + 3' instead of '( 1 + 2 ) + 3'.  Current logic is a
 * bit kludgy.
 */
void
eval(struct expression *exp)
{
	int c = 0;
	uint32_t m = exp->mask;
	char *ops = exp->operators;
	struct element *sp = exp->stack;

	while( m ){
		if( m & 0x1 ){ /* Apply an operator */
			char buf[1024];
			char fmt[32];

			assert(sp - exp->stack > 1); /* True because of mask_is_invalid() */
			assert(sizeof buf >= sizeof sp->descr); /* Ensure terminating null after strncpy */

			compute_fmt(sp - 2, fmt, *ops, m == 1);
			sp -= 2;
			sp->last = *ops;
			snprintf(buf, sizeof buf, fmt, sp->descr, *ops, sp[1].descr);
			strncpy(sp->descr, buf, sizeof sp->descr);
			switch( *ops++ ){
			case '+': sp->val += sp[1].val; break;
			case '-': sp->val -= sp[1].val; break;
			case '*': sp->val *= sp[1].val; break;
			case '/': sp->val /= sp[1].val; break;
			default: assert(0);
			}
		} else {
			assert( c < exp->count );
			sp->val = exp->operands[c++];
			sp->last = '.';
			snprintf(sp->descr, sizeof sp->descr, "%g", sp->val);
		}
		sp += 1;
		assert( sp - exp->stack <= exp->count );
		m >>= 1;
	}
	sp -= 1;
	printf("%s = %g\n", sp->descr, sp->val);
	return;
}


/*
 * A mask is invalid (will lead to underflow) unless it meets the
 * following condition: For any given bit, the number of unset bits
 * to the right of it must be greater than the number of set bits.
 */
int
mask_is_invalid(uint32_t m)
{
	int sum = 0;
	do {
		sum += ( m & 0x1 ) ? 1 : -1;
		m >>= 1;
	} while( m && sum < 0 );
	return sum >= 0;
}


/*
 * generate the next mask with N bits set.  See:
 * https://stackoverflow.com/questions/26594951/finding-next-bigger-number-with-same-number-of-set-bits
 */
uint32_t
next_mask(uint32_t x)
{
	do {
		uint32_t c = x & -x;
		uint32_t r = x + c;
		x = (((r ^ x) >> 2) / c) | r;
	} while( mask_is_invalid(x) );
	return x;
}

/* Given a string of operators (eg "+*-/++"), generate the next permutation */
void
next_perm(char *s)
{
	size_t len = strlen(s);
	assert( strspn( s, "+-*/" ) == len );
	for( int i = 0; i < (int)len; i++ ){
		switch( s[i] ){
		case '+': s[i] = '-'; return;
		case '-': s[i] = '*'; return;
		case '*': s[i] = '/'; return;
		case '/': s[i] = '+'; break;
		}
	}
}


int
next_op(struct expression *exp)
{
	int N = exp->count - 1;
	if( strspn(exp->operators, "/") == (unsigned)N ){
		exp->mask = next_mask(exp->mask);
		assert( exp->mask >= 1 << 2*N );
	}
	next_perm(exp->operators);
	return exp->mask < 1 << (2*N + 1);
}


/*
 * Initialize exp from the command line arguments.
 */
void
parse_cmd_line(int argc, char **argv, struct expression *exp)
{
	double *v;
	if( argc < 3 || argc > 14 ){
		errx(EXIT_FAILURE, "Invalid call: must specify between 2 and 14 numeric values");
	}

	argv += 1;
	argc -= 1;
	exp->operands = v = xmalloc(sizeof *v * argc);
	exp->count = argc;

	for( ;*argv; argv++, v++ ){
		char *end;
		*v = strtod(*argv, &end);
		if( *end != '\0' ){
			errx(EXIT_FAILURE, "Invalid input in \"%s\""
				"at position %ld.  Unexpected value: '%c'",
				*argv, end - *argv, *end);
		}
	}
	strncpy(exp->operators, "++++++++++++++++++++", exp->count - 1);
	exp->operators[exp->count] = '\0';
	exp->mask = (( 0x1 << ( exp->count - 1 )) - 1);
	exp->mask = next_mask(exp->mask);
	exp->operands = exp->operands;
	exp->stack = xmalloc( exp->count * sizeof *exp->stack);
}


void *
xmalloc(size_t s)
{
	void *r = malloc(s);
	if( r == NULL ){
		err(EXIT_FAILURE, "malloc");
	}
	return r;
}
