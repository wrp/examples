/*
 * A simple reverse polish calculator
 */

#define siz(x) ( (sizeof (x) / sizeof *(x)))

#include <stdio.h>
#include <ctype.h>

int
main(int argc, char **argv)
{
	int c;
	double stack[1024];

	double *sp = stack + 1;
	int new=1;
	int width = 6;
	int precision = 3;
	char fmt[32];

	while( (c=getchar()) != EOF ) {
		switch(c) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			if( new ) *(++sp) = 0.0;
			*sp = 10.0 * *sp + (c - '0'); break;
		case '*':
			sp -= 1; sp[0] *= sp[1]; break;
		case ' ': case '\n': case '\t': case ',':
			break;
		case '+':
			sp -= 1; sp[0] += sp[1]; break;
		case '/':
			sp -= 1; sp[0] /= sp[1]; break;
		case '-':
			sp -= 1; sp[0] -= sp[1]; break;
		case 'k':
			precision = *sp--; break;
		case 'w':
			width = *sp--; break;
		case 'p':
			snprintf(fmt, sizeof fmt, "%%%d.%dg\n", width, precision);
			printf(fmt, sp[0]);
			break;
		default:
			fprintf(stderr, "Unrecognized value: %c\n", c);
		case 'q':
			goto end;
		}
		new = !isdigit(c);
		if( sp == stack || sp - stack >= siz(stack)) {
			fprintf(stderr, "%sflow\n", sp == stack ? "Under" : "Over");
			goto fail;
		}
	}
end:
	return 0;
fail:
	return 1;
}
