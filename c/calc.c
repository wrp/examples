/*
 * A simple reverse polish calculator
 */

#define siz(x) ( (sizeof (x) / sizeof *(x)))

#include <stdio.h>
#include <ctype.h>

struct state {
	double stack[1024];
	double *sp;
	char buf[32];
	char *bp;
	int width;
	int precision;
};

int
main(int argc, char **argv)
{
	int c;
	struct state S;
	char fmt[32];
	int new = 1;

	S.sp = S.stack + 1;
	S.width = 6;
	S.precision = 3;

	while( (c=getchar()) != EOF ) {
		switch(c) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '.': case 'e': case 'E': case '_':
			if( new ) *(++S.sp) = 0.0;
			*S.sp = 10.0 * *S.sp + (c - '0'); break;
		case '*':
			S.sp -= 1; S.sp[0] *= S.sp[1]; break;
		case ' ': case '\n': case '\t': case ',':
			break;
		case '+':
			S.sp -= 1; S.sp[0] += S.sp[1]; break;
		case '/':
			S.sp -= 1; S.sp[0] /= S.sp[1]; break;
		case '-':
			S.sp -= 1; S.sp[0] -= S.sp[1]; break;
		case 'k':
			S.precision = *S.sp--; break;
		case 'w':
			S.width = *S.sp--; break;
		case 'p':
			snprintf(fmt, sizeof fmt, "%%%d.%dg\n", S.width, S.precision);
			printf(fmt, S.sp[0]);
			break;
		default:
			fprintf(stderr, "Unrecognized value: %c\n", c);
		case 'q':
			goto end;
		}
		new = !isdigit(c);
		if( S.sp == S.stack || S.sp - S.stack >= siz(S.stack)) {
			fprintf(stderr, "%sflow\n", S.sp == S.stack ? "Under" : "Over");
			goto fail;
		}
	}
end:
	return 0;
fail:
	return 1;
}
