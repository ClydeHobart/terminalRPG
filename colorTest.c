#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "libs/constants.h"
#include "libs/color.h"

int main(void)
{
	short red, grn, blu;
	// short fg, bg;
	short ***initials;

	initscr();
	start_color();
	// use_default_colors();
	curs_set(0);
	// mvprintw(38, 0, "COLORS: %d, COLOR_PAIRS: %d", COLORS, COLOR_PAIRS);
	initials = initColors();

	for (int r = 0; r < 2; r++) {
		move(r, 0);

		for (int c = 0; c < 8; c++) {
			attron(COLOR_PAIR(r * 8 + c));
			color_content((short)(r * 8 + c), &red, &grn, &blu);
			printw("##");
		}
	}

	for (int r = 0; r < 40; r++) {
		move(r + 2, 0);

		for (int c = 0; c < 6; c++) {
			attron(COLOR_PAIR(r * 6 + c + 16));
			color_content((short)(r * 6 + c + 16), &red, &grn, &blu);
			printw("##");
		}
	}

	// int colors[33] = {YTU_COLOR_BLACK, YTU_COLOR_GRAY, YTU_COLOR_WHITE, 196, 202, 208, 214, 220, 226, 190, 154, 118, 82, 46, 47, 48, 49, 50, 51, 45, 39, 33, 27, 21, 57, 93, 129, 165, 201, 200, 199, 198, 197};
	int colors[33] = {YTU_COLOR_BLACK, YTU_COLOR_GRAY, YTU_COLOR_WHITE, YTU_COLOR_RED, YTU_COLOR_YELLOW, YTU_COLOR_GREEN, YTU_COLOR_CYAN, YTU_COLOR_BLUE, YTU_COLOR_MAGENTA};
	for (int dc = 4; dc > -5; dc--) {
		move (4 - dc, 20);

		// for (int c = 0; c < 33; c++) {
		for (int c = 0; c < 9; c++) {
			attron(COLOR_PAIR(adjustColor(colors[c], dc)));
			// addch(ACS_CKBOARD);
			printw("%.3d ", adjustColor(colors[c], dc));
		}
	}

	getch();
	resetColors(initials);
	refresh();
	getch();
	endwin();
	return 0;
}