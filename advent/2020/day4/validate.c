#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

/* sample input
ecl:hzl byr:1926 iyr:2010 pid:221225902 cid:61 hgt:186cm eyr:2021 hcl:#7d3b0c
hcl:#efcc98 hgt:178 pid:433543520 eyr:2020 byr:1926 ecl:blu cid:92 iyr:2010
iyr:2018 eyr:2026 byr:1946 ecl:brn hcl:#b6652a hgt:158cm pid:822320101
*/

struct passport {
	char byr[32]; /* (Birth Year)*/
	char iyr[32]; /* (Issue Year) */
	char eyr[32]; /* (Expiration Year) */
	char hgt[32]; /* (Height) */
	char hcl[32]; /* (Hair Color) */
	char ecl[32]; /* (Eye Color) */
	char pid[32]; /* (Passport ID) */
	char cid[32]; /* (Country ID) */
};

static int
get_entry(struct passport *p, int *valid)
{
	char key[32];
	char val[32];
	int rv;
	int invalid = 0;
	char *v;
	memset(p, 0, sizeof *p);
	while( (rv = scanf("%31[^:]:", key)) == 1 ){
		char delim;
		if( !strcmp(key, "byr") ){
			v = p->byr;
		} else if( !strcmp(key, "iyr") ){
			v = p->iyr;
		} else if( !strcmp(key, "eyr") ){
			v = p->eyr;
		} else if( !strcmp(key, "hgt") ){
			v = p->hgt;
		} else if( !strcmp(key, "hcl") ){
			v = p->hcl;
		} else if( !strcmp(key, "ecl") ){
			v = p->ecl;
		} else if( !strcmp(key, "pid") ){
			v = p->pid;
		} else if( !strcmp(key, "cid") ){
			v = p->cid;
		} else {
			v = val;
		}
		if( *v ){
			invalid = 1;
			v = val;
		}
		if( 2 != scanf("%31[^ \n]%c", v, &delim) ){
			errx(1, "Invalid input");
		}
		if( delim == '\n' ){
			break;
		}
	}

	int d = strtol(p->byr, NULL, 10);
	if( d < 1920 || d > 2002 ){
		invalid = 1;
	}
	d = strtol(p->iyr, NULL, 10);
	if( d < 2010 || d > 2020 ){
		invalid = 1;
	}
	d = strtol(p->eyr, NULL, 10);
	if( d < 2020 || d > 2030 ){
		invalid = 1;
	}
	d = strtol(p->hgt, &v, 10);
	if( !strcmp(v, "cm") ){
		if( d < 150 || d > 193 ){
			invalid = 1;
		}
	} else if( !strcmp(v, "in") ){
		if( d < 59 || d > 76 ){
			invalid = 1;
		}
	} else {
		invalid = 1;
	}

	if( *p->hcl != '#' || strspn(p->hcl + 1, "0123456789abcdef") != 6 ){
		invalid = 1;
	}

	/*
byr (Birth Year) - four digits; at least 1920 and at most 2002.
iyr (Issue Year) - four digits; at least 2010 and at most 2020.
eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
hgt (Height) - a number followed by either cm or in:
  If cm, the number must be at least 150 and at most 193.
  If in, the number must be at least 59 and at most 76.
hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
pid (Passport ID) - a nine-digit number, including leading zeroes.
cid (Country ID) - ignored, missing or not.
*/
	if( !strcmp(p->ecl, "amb")
		|| !strcmp(p->ecl, "blu")
		|| !strcmp(p->ecl, "brn")
		|| !strcmp(p->ecl, "gry")
		|| !strcmp(p->ecl, "grn")
		|| !strcmp(p->ecl, "hzl")
		|| !strcmp(p->ecl, "oth") ){
		invalid = 1;
	}
	d = strtol(p->pid, NULL, 10);
	if( d < 0 || d > 999999999 ){
		invalid = 1;
	}
	if( !invalid ){
		*valid += 1;
	}
	return rv == 1;
}

int
main(void)
{

	int count = 0;
	struct passport a = {0};
	while( get_entry(&a, &count) ){
		;
	}
	printf("%d\n", count);
	return 0;
}
