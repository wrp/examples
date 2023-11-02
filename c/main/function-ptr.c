
#include <stdio.h>
#include <stdlib.h>

/* Extremely silly example showing a function pointer */

struct x;
typedef double op(const struct x, const struct x);

struct x {
	double value;
	op *op;
};

op add;
op subtract;
op multiply;
op divide;
double print(const struct x a, const struct x b) {
	(void)b;
	printf("%g\n", a.value);
	return 0;
}

int
main(void)
{
	struct x a = { 6, multiply };
	struct x b = { 9, NULL };
	struct x c = { a.op(a, b), print };
	c.op(c, c);
	return 0;
}


double add(const struct x a, const struct x b) { return a.value + b.value; }
double subtract(const struct x a, const struct x b) { return a.value - b.value; }
double multiply(const struct x a, const struct x b) { return a.value * b.value; }
double divide(const struct x a, const struct x b) { return a.value / b.value; }

