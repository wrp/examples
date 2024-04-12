
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
/*
 * Play around with scanf to parse a monomial
 * Some sample inputs:
 *   2x^3
 *   -2
 *   2x
 *   x
 */

struct term {
	double coeff;
	int exponent;
};

int
parse(const char *m, struct term *result)
{
	char *end;

	result->exponent = 0;
	while( isspace(*m) )
		m+=1;
	result->coeff = strtod(m, &end);
	if(*end == 'x' && end == m)
		result->coeff = 1;
	while( isspace(*end) )
		end+=1;
	switch(*end++) {
	case '\0': return 0;
	case 'x': break;
	default: return -1;
	}
	switch(*end++) {
	case '\0':
		result->exponent = 1;
		return 0;
	case '^':
		result->exponent = strtol(end, &end, 10);
		break;
	default: return -1;
	}
	while( isspace(*end) )
		end+=1;
	return *end != '\0';
}

int
main(int argc, char **argv)
{
	(void)argc;
	for(argv += 1; *argv; argv++) {
		struct term result;
		parse(*argv, &result);
		printf("%s:\t%lg,%d\n", argv[0], result.coeff, result.exponent);
	}
}
