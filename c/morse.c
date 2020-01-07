
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

void
text_to_morse(char c, FILE *out)
{
		if( islower(c)) {
			fputs(alphamorse[c - 'a'], out);
			fputc(' ', out);
		} else if( isdigit(c)) {
			fputs(nummorse[c - '0'], stdout);
			fputc(' ', out);
		} else if( isspace(c)) {
			fputc(c, out);
		} else {
			fputc(' ', out);
		}
}

int
main(void)
{
	int c;
	while( ( c = tolower(getchar())) != EOF ) {
		text_to_morse(c, stdout);
	}
}
