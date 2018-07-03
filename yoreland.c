#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "libs/animations.h"

int main(void)
{
	WINDOW *win;

	initscr();
	curs_set(0);
	win = newwin(40, 120, 0, 0);
	refresh();
	box(win, 0, 0);
	wrefresh(win);

	int oTReturn = openingTitle_anim(win);
	printw("%d", oTReturn);

	getch();
	endwin();
	return 0;
}