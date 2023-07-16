#include <stdio.h>

int fool(long double x) { return printf("long double %Lg\n", x); }
int food(int x) { return printf("int %d\n", x); }
int foof(float x) { return printf("float %g\n", x); }
int fooc(char *x) { return printf("char * %s\n", x); }

#define foo(X) _Generic((X), \
	long double: fool,   \
	char *: fooc,        \
	default: food,       \
	float: foof          \
)(X)


#define print_type(s) do { \
	printf(#s " at %p: ", &s); \
	if( __builtin_types_compatible_p(__typeof__(s), int) ){ \
		puts("int"); \
	} else if( __builtin_types_compatible_p(__typeof__(s), long) ){ \
		puts("long"); \
	} else if( __builtin_types_compatible_p(__typeof__(s), char *) ){ \
		puts("char *"); \
	} else if( __builtin_types_compatible_p(__typeof__(s), float) ){ \
		puts("float"); \
	} else { \
		puts("unknown type"); \
	} \
}while(0)


int
main(void)
{
	int z;

	z = foo("2.0");
	z = foo((float)2.0);
	z = foo((long double)2.0);
	z = foo(2);

	print_type(z);
	char *a;
	print_type(a);

	return z == 0;
}
