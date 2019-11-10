
#include <stdio.h>
#include <stdlib.h>

/* Extremely silly example showing a function pointer */

struct x {
	double value;
	double (*op)(struct x, struct x);
};

double add(const struct x a, const struct x b);
double subtract(const struct x a, const struct x b);
double multiply(const struct x a, const struct x b);
double divide(const struct x a, const struct x b);
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

