/* Demonstrate a generic funcion pointer */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void addi(int a, int b) { printf("%d + %d = %d\n", a, b, a + b); }
static void addf(double a, double b) { printf("%f + %f = %f\n", a, b, a + b); }

union func {
	void (*i)(int, int);
	void (*f)(double, double);
};

union v { int i; double f; };

struct data {
	union v a;
	union v b;
	int type;
};

void
foo(struct data *d, union func *f)
{
	switch( d->type ){
	case 0: f->i(d->a.i, d->b.i); break;
	case 1: f->f(d->a.f, d->b.f);
	}
}

int
main(void)
{
	struct data d;
	union func f;

	d.type = 0;
	d.a.i = 2;
	d.b.i = 3;
	f.i = addi;
	foo(&d, &f);

	d.type = 1;
	d.a.f = 3.4;
	d.b.f = 2.783;
	f.f = addf;
	foo(&d, &f);

	return 0;
}
