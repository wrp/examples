#include <stdarg.h>

/* A trivial library used to demonstrate bindgen to wrap C from rust */

int
sum_2_int(int x, int y)
{
	return x + y;
}

/* Sum non-zero int values */
int
sum_int(int x, ...)
{
	int sum = x;
	int arg = x;
	va_list ap;
	va_start(ap, x);
	while( arg != 0 ){
	arg = va_arg(ap, int);
	sum += arg;
	}
	va_end(ap);
	return sum;
}
