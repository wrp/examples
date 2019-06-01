/*
 * A simple reverse polish calculator
 * Plenty of stack overflows possible here!
 */


#include <stdio.h>

int
main(int argc, char **argv)
{
	int c;
	double stack[1024];

	double *sp = stack - 1;
	int prev = ' ';
	int width = 6;
	int precision = 3;
	char fmt[32];

#ifndef DEBUG
	while( (c=getchar()) != EOF ) {
#else
	int input[] = {'1', '2', ',', '6', '/', 'p', 0 };
	for( int *k = input; (c = *k) != 0; k++ ) {
#endif
		switch(c) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			if( prev == ' ') *(++sp) = 0.0;
			*sp = 10.0 * *sp + (c - '0'); break;
		case '*':
			sp -= 1; sp[0] *= sp[1]; break;
		case ' ': case '\n': case '\t': case ',':
			c = ' '; break;
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
		prev = c;
	}
end:
	return 0;
}
