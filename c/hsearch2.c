/* Store words in input in a hash table */
#include <limits.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
	char str[512];

	ENTRY item;

	if( ! hcreate(65536) ){
		perror("hcreate");
		return 1;
	}

	while( scanf("%511s", str) == 1 ){
		item.key = str;
		ENTRY *this = hsearch(item, FIND);
		if( this ){
			*(int *)this->data += 1;
		} else {
			int *x = malloc(sizeof *x);
			if( x == NULL ){
				perror("malloc");
				exit(1);
			}
			item.key = strdup(str);
			item.data = x;
			*x = 1;
			if( hsearch(item, ENTER) == NULL ){
				perror("hsearch");
				return 1;
			}
		}
	}

	/* Access table. */
	for(int i = 0; i < argc; i++ ){
		item.key = argv[i];
		ENTRY *e;
		if( (e = hsearch(item, FIND)) != NULL ){
			printf("%s: %d\n", argv[i], *(int *)e->data);
		} else {
			printf("%s was not found\n", argv[i]);
		}
	}
	hdestroy();
	return 0;
}
