#include <stdio.h>
#include <stdlib.h>
#include <err.h>

struct pos { int y; int x; };

static void
move(int count, struct pos *p, const struct pos *w)
{
	p->y += w->y * count;
	p->x += w->x * count;
}

static void
move_way(char dir, int count, struct pos *p)
{
	switch( dir ){
	case 'N': p->y -= count; break;
	case 'S': p->y += count; break;
	case 'E': p->x += count; break;
	case 'W': p->x -= count; break;
	}
}


static void
rotate(int count, struct pos *w)
{
	int wy = w->y;
	int wx = w->x;
	switch( count ){
	case 90:
	case -270:
		w->y =  wx;
		w->x = -wy;
		break;
	case 180:
	case -180:
		w->y = -wy;
		w->x = -wx;
		break;
	case -90:
	case 270:
		w->y = -wx;
		w->x =  wy;
		break;
	}
}

int
main(void)
{
	int count;
	char c;
	struct pos p = { 0, 0 };
	struct pos w = { -1, 10 }; /* 10 east, 1 north */
	while( scanf("%c%d ", &c, &count) == 2 ){
		switch( c ){
		case 'F':
			move(count, &p, &w);
			break;
		case 'N':
		case 'S':
		case 'E':
		case 'W':
			move_way(c, count, &w);
			break;
		case 'L': rotate(-count, &w); break;
		case 'R': rotate(count, &w); break;
		}
		#ifdef DEBUG
		printf("%c: %d\tship@(%3d,%3d)\tway@(%3d,%3d)\n",
			c, count, p.y, p.x, w.y, w.x);
		#endif
	}
	printf("pos = %d, %d  distance = %d\n", p.y, p.x, abs(p.y) + abs(p.x));
	return 0;
}
