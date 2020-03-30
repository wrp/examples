#include "scanf.h"
#include "xutil.h"
#include <string.h>


void
validate(const char *fmt, size_t width, int conv, const char *flags, int end)
{
	int k;
	struct conversion_specifier cs;

	k = parse_format_string(fmt, &cs);
	if(!k) {
		die("Failed to parse %s\n", fmt);
	}
	if( cs.width != width ) {
		die("%s: Invalid width:  %zu != %zu\n", fmt, cs.width, width);
	}
	if( cs.conversion[0] != conv ) {
		die("%s: Invalid specifier:  '%c' != '%c'\n",
			fmt, *cs.conversion, conv);
	}
	if( strncmp(flags, cs.flags, strlen(flags)) ) {
		die("%s: Invalid flags:  '%s' != '%s'\n", fmt, cs.flags, flags);
	}
	if( end != *cs.e ) {
		die("%s: Invalid end:  '%c' != '%c'\n", fmt, *cs.e, end);
	}
}

int
main(void)
{
	validate("...%17dX...", 17, 'd', "", 'X');
	validate("...%17ldX...", 17, 'd', "l", 'X');
	validate("...%7LgX...", 7, 'g', "L", 'X');
	validate("...%7s...", 7, 's', "", '.');
	validate("...%7[^kdj]...", 7, '[', "", '.');
}
