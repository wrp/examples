#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Note: strtok is obsoleted by strsep */
struct person {
	char *id;
	char *name;
	char *age;
	char *grade;
	char *country;
	char *city;
};


int
main(void)
{
	char *line = NULL;
	size_t s;
	struct person p;

	while( getline(&line, &s, stdin) > 0 ) {
		char *t = line;
		p.id = strsep(&t, ",");
		p.name = strsep(&t, ",");
		p.age = strsep(&t, ",");
		p.grade = strsep(&t, ",");
		p.country = strsep(&t, ",");
		p.city = strsep(&t, ",");
		printf("grade: %s\n", p.grade);
		printf("city: %s\n", p.city);
	}
}
