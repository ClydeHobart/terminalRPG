#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "libs/color.h"

extern const int C_BLACK;
extern const int C_GRAY;
extern const int C_WHITE;
extern const int C_RED;
extern const int C_ORANGE;
extern const int C_YELLOW;
extern const int C_CHARTREUSE;
extern const int C_GREEN;
extern const int C_SPRING_GREEN;
extern const int C_CYAN;
extern const int C_AZURE;
extern const int C_BLUE;
extern const int C_VIOLET;
extern const int C_MAGENTA;
extern const int C_HOT_PINK;

int main(void)
{
	char **colors;
	initscr();
	curs_set(0);
	colors = initColors();
	for (int dc = 3; dc >-4; dc--) {

		for (int r = 0; r < 13; r++) {
			printf("%s#", colors[8 * r + dc + 4]);
		}
		printf("\x1b[0m\n");
	}
	refresh();
	getch();
	endwin();
	printf("%sorange test%s", "\x1b[38;2;255;127;0m", "\x1b[0m\n");
	return 0;
}