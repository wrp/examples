/* Simple example using function pointers */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b) { return a / b; }

struct func {
	char descr[32];
	char op;
	double (*f)(double, double);
};

int
main(int argc, char **argv)
{
	double a, b;
	char option = argc > 2 ? tolower(*argv[2]) : '+';
	char *lut = "+-*/";
	char *opt = strchr(lut, option);
	int idx = opt == NULL ? -1 : opt - lut;
	struct func table[] = {
		{"Addition", '+', add},
		{"Subtraction", '-', subtract},
		{"Multiplication", '*', multiply},
		{"Division", '/', divide}
	};
	struct func *f = table + idx;

	a = argc > 1 ? strtod(argv[1], NULL) : 10;
	b = argc > 3 ? strtod(argv[3], NULL) : 20;

	if(idx == -1) {
		fprintf (stderr, "Invalid option: '%c'\n", option);
		return EXIT_FAILURE;
	}
	f = table + idx;
	printf("%s: %g %c %g = %g\n" , f->descr, a, option, b, f->f(a,b));

	return 0;
}
