
/* Convert stdin to morse
 *
 * conversions taken from https://stackoverflow.com/questions/59588239/c-function-for-converting-text-to-morse-code
 */

#include <stdio.h>
#include <ctype.h>

char *alphamorse[] = {
	".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", /* A - J */
	"-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", /* K - T */
	"..-", "...-", ".--", "-..-", "-.--", "--.." /* W - Z */
};
char *nummorse[]={
	"-----", ".----", "..---", "...--", "....-",
	".....", "-....", "--...", "---..", "----."
};
int
main(void)
{
	int c;
	while( ( c = tolower(getchar())) != EOF ) {
		if( islower(c)) {
			fputs(alphamorse[c - 'a'], stdout);
			putchar(' ');
		} else if( isdigit(c)) {
			fputs(nummorse[c - '0'], stdout);
			putchar(' ');
		} else if( isspace(c)) {
			putchar(c);
		} else {
			putchar(' ');
		}
	}
}
