
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <err.h>

int answer[26];
int
get_count(int c)
{
	int count = 0;
	for( int i = 0; i < 26; i++ ){
		if( answer[i] == c ){
			count += 1;
		}
	}
	return count;
}

int
main(void)
{
	int c;
	int people_in_group = 0;
	int sum = 0;
	int nl = 0;
	while( (c = getchar()) != EOF ){
		if( isspace(c) ){
			if( !nl ){
				people_in_group += 1;
			}
		} else if( c < 'a' || c > 'z' ){
			errx(1, "Invalid input");
		} else {
			answer[c - 'a'] += 1;
		}
		if( c == '\n' && nl ){ /* New group */
			sum += get_count(people_in_group);
			people_in_group = 0;
			memset(answer, 0, sizeof answer);
		}
		nl = c == '\n';
	}
	printf("%d\n", sum + get_count(people_in_group));

}
