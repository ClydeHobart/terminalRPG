#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "libs/file.h"

int main(void)
{
	FILE *file;
	WINDOW *win;
	int mapRows, mapCols;
	int mapRow = 0, mapCol = 0;
	int viewRows, viewCols;
	int keyPress;
	char **map;
	char *mapRowsStr, *mapColsStr;

	file = fopen("assets/maps/testMap0.txt", "r");

	if (file == NULL)
	{
		printf("testMap0 couldn't load\n");
		return 1;
	}

	mapRowsStr = readlinep(file);
	mapColsStr = readlinep(file);
	mapRows = atoi(mapRowsStr);
	mapCols = atoi(mapColsStr);
	free(mapRowsStr);
	free(mapColsStr);
	map = malloc(mapRows * sizeof(char *));

	if (map == NULL)
	{
		fprintf(stderr, "map couldn't malloc\n");
		return 2;
	}

	for (int r= 0; r < mapRows; r++)
	{
		map[r] = readlinep(file);
	}

	initscr();
	curs_set(0);
	
	box(stdscr, 0, 0);
	mvprintw(1, 1, "testMap0 is %d rows x %d cols", mapRows, mapCols);
	viewRows = 8;
	viewCols = 16;
	win = newwin(10, 18, viewRows + 2, viewCols + 2);
	keypad(win, true);
	box(win, 0, 0);
	refresh();
	wrefresh(win);

	do
	{
		for (int r = 0; r < viewRows; r++)
		{
			mvwprintw(win, r + 1, 1, "%.*s", viewCols, map[mapRow + r] + mapCol);
		}
		
		keyPress = wgetch(win);
		mvprintw(2, 1, "keyPress: %d", keyPress);
		mvprintw(3, 1, "KEY_UP: %d,\tKEY_DOWN: %d,\tKEY_LEFT: %d,\tKEY_RIGHT: %d",
			KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
		refresh();
		wrefresh(win);

		if (keyPress == KEY_UP && mapRow > 0) mapRow--;
		else if (keyPress == KEY_DOWN && mapRow < mapRows - viewRows) mapRow++;
		else if (keyPress == KEY_LEFT && mapCol > 0) mapCol--;
		else if (keyPress == KEY_RIGHT && mapCol < mapCols - viewCols) mapCol++;
	}
	while (keyPress != 'X');
	endwin();
	return 0;
}