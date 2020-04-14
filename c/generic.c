#include <stdio.h>

int fool(long double x) {
	return printf("long double %Lg\n", x);
}
int food(int x) {
	return printf("int %d\n", x);
}
int foof(float x) {
	return printf("float %g\n", x);
}
int fooc(char *x) {
	return printf("char * %s\n", x);
}

#define foo(X) _Generic((X), \
	long double: fool,   \
	char *: fooc,   \
	default: food,       \
	float: foof          \
	)(X)

int main(void) {
	int z;

	z = foo("2.0");
	z = foo((float)2.0);
	z = foo((long double)2.0);
	z = foo(2);
	return 0;
}
