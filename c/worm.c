/* ncurses implementation of worm */

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define DERECHA 1
#define IZQUIERDA 2
#define ARRIBA 3
#define ABAJO 4

void
init(void)
{
	initscr();
	noecho();
	curs_set(0);
	srand(time(NULL));
	nodelay(stdscr, TRUE);
}

int
incrementapuntos(int puntos, int tipocomida)
{
	char incremento[] = {1, 3, 5};
	move(0, 0);
	printw("%d", puntos + incremento[tipocomida]);

	return puntos + incremento[tipocomida];
}

void
draw_board(char ra, char ca, char rc, char cc)
{
	ra = 8;
	ca = 10;
	while (ra <= 15) {
		while (ca <= 50) {
			move(ra, ca);
			printw("-");
			ca = ca + 1;
		}
		ra = ra + 1;
		ca = 10;
	}
	ra = 5;
	ca = 30;
	move(ra, ca);
	printw("*");
	//impresion de la comida
	int		puntos = 0;
	int		tipocomida = rand() % 3;
	rc = rand() % 22;
	cc = rand() % 60;
	move(rc, cc);
	if (tipocomida == 0) {
		printw("c");
	}
	if (tipocomida == 1) {
		printw("f");
	}
	if (tipocomida == 2) {
		printw("x");
	}
	move(0, 0);
	printw("%d", puntos);
	refresh();
}

int
finalizar(void)
{
	endwin();
	return 0;
}

void
operacion(char ra, char ca)
{
	ra = 5;
	ca = 30;
	int		tecla = 0;
	int		direccion = IZQUIERDA;
	while (tecla != 'q') {
		tecla = getch();
		move(ra, ca);
		printw("  ");
		switch( tecla ){
		case 'e': case 'i': direccion = ARRIBA; break;
		case 's': case 'j': direccion = IZQUIERDA; break;
		case 'd': case 'k': direccion = ABAJO; break;
		case 'f': case 'l': direccion = DERECHA; break;
		}
		move(ra, ca);
		printw("  ");

		if (direccion == ARRIBA) {
			ra = ra - 1;
			if (ra <= 0) {
				ra = 21;
			}
		}
		if (direccion == ABAJO) {
			ra = ra + 1;
			if (ra >= 22) {
				ra = 0;
			}
		}
		if (direccion == DERECHA) {
			ca = ca + 1;
			if (ca >= 60) {
				ca = 0;
			}
		}
		if (direccion == IZQUIERDA) {
			ca = ca - 1;
			if (ca <= 0) {
				ca = 59;
			}
		}
		move(ra, ca);
		printw("*");
		refresh();
		usleep(400000);

		//Variables del siguiente ciclo
		char		rc = 0;
		char		cc = 0;
		int		puntos = 0;
		int		tipocomida = 0;
		if (ra == rc && ca == cc) {

			puntos = incrementapuntos(puntos, tipocomida);
			tipocomida = rand() % 3;
			rc = rand() % 22;
			cc = rand() % 60;
			move(rc, cc);
			if (tipocomida == 0) {
				printw("c");
			}
			if (tipocomida == 1) {
				printw("f");
			}
			if (tipocomida == 2) {
				printw("x");
			}
			refresh();
			usleep(8000000);
		}
		if (ra >= 8 && ra <= 15 && ca >= 10 && ca <= 50) {
			break;
		}
	}
	refresh();
}
int
main(void)
{
	char ra = 0;
	char ca = 0;
	char rc = 0;
	char cc = 0;
	char tipocomida = 0;
	int puntos = 0;

	init();
	draw_board(ra, ca, rc, cc);
	operacion(ra, ca);
	incrementapuntos(puntos, tipocomida);
	return finalizar();
}
