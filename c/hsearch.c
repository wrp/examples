#include <limits.h>
#include <search.h>
#include <stdio.h>
#include <string.h>

#define NUM_EMPL 100


struct info {
	char *key;
	int age;
	int room;
	struct info *data;
};

int
main(void)
{
	char str[BUFSIZ];

	struct info info_space[NUM_EMPL];
	struct info *info_ptr = info_space; /* Next space in info_space. */
	ENTRY item;
	ENTRY *found_item; /* Name to look for in table. */
	char name_to_find[30];
	int i = 0;

	/* Create table; no error checking is performed. */
	(void) hcreate(NUM_EMPL);

	while( scanf("%s%d%d", str, &info_ptr->age,
			&info_ptr->room) == 3 && i++ < NUM_EMPL ){
		/* Put information in structure, and structure in item. */
		item.key = strdup(str);
		item.data = info_ptr;
		info_ptr++;
		/* Put item into table. */
		(void) hsearch(item, ENTER);
	}

	/* Access table. */
	item.key = name_to_find;
	while( scanf("%s", item.key) != EOF ){
		if ((found_item = hsearch(item, FIND)) != NULL) {
			/* If item is in the table. */
			(void)printf("found %s, age = %d, room = %d\n",
				 found_item->key,
				 ((struct info *)found_item->data)->age,
				 ((struct info *)found_item->data)->room
			);
		} else {
			(void)printf("no such employee %s\n", name_to_find);
		}
	}
	hdestroy();
	return 0;
}
