#include <stdio.h>
#include <err.h>
/*
F35
L90
S5
F4
R90
Action N means to move north by the given value.
Action S means to move south by the given value.
Action E means to move east by the given value.
Action W means to move west by the given value.
Action L means to turn left the given number of degrees.
Action R means to turn right the given number of degrees.
Action F means to move forward by the given value in the direction the ship is currently facing.
*/

struct pos { int y; int x; };

static void
move(char dir, int count, struct pos *p)
{
	switch( dir ){
	case 'N': p->y -= count; break;
	case 'S': p->y += count; break;
	case 'E': p->x += count; break;
	case 'W': p->x -= count; break;
	}
}

static void
rotate(int d, int count, int *o)
{
	if( d == 1 ){
		*o -= count;
	} else {
		*o += count;
	}
	while( *o < 0 ){
		*o += 360;
	}
	*o %= 360;
}

int
main(void)
{
	int count;
	char c;
	int orientation = 90;
	struct pos p = { 0, 0 };
	while( scanf("%c%d ", &c, &count) == 2 ){
		switch( c ){
		case 'F':
			switch( orientation ){
			case  90: c = 'E'; break;
			case 180: c = 'S'; break;
			case 270: c = 'W'; break;
			case   0: c = 'N'; break;
			default: errx(1, "Bad orientation: %d", orientation);
			}
		case 'N':
		case 'S':
		case 'E':
		case 'W':
			move(c, count, &p);
			break;
		case 'L': rotate(1, count, &orientation); break;
		case 'R': rotate(0, count, &orientation); break;
		}
	}
	printf("pos = %d, %d\n", p.x, p.y);
	return 0;
}
