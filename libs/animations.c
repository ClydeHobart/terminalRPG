/* animations.c
 * A set of functions to perform animations for YTU. See animations.h for more
 * information regarding individual functions
 *
 * Zeke Baker | 20180702.1038 | terminalRPG/libs/animations.c
 */

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "file.h"

/* **************** global functions ****************
 * See animations.h for function descriptions
 */

/**************** openingTitle_anim() ****************/
int openingTitle_anim(WINDOW* win);

/**************** local functions ****************/

/**************** openingTitle_anim() ****************/
int openingTitle_anim(WINDOW* win)
{
	const struct timespec Y_SLEEP = {0, 20000000L};
	const struct timespec T_U_SLEEP = {0, 100000000L};
	const int Y_ROW = 6;
	const int Y_COL = 12;
	const int Y_HEIGHT = 21;
	const int Y_WIDTH = 96;
	const int T_U_ROW = 30;
	const int T_U_COL = 27;
	const int T_U_HEIGHT = 5;
	const int T_U_WIDTH = 65;

	FILE *file;
	char **title;
	char **yoreland;
	char **timeUntold;

	file = fopen("assets/yoreland.txt", "r");

	if (file == NULL)
	{
		return 1;
	}

	title = calloc(40, sizeof(char *));
	yoreland = calloc(21, sizeof(char *));
	timeUntold = calloc(5, sizeof(char *));

	for (int r = 0; r < 40; r++)
	{
		title[r] = readlinep(file);

		if (r >= Y_ROW && r < Y_ROW + Y_HEIGHT)
		{
			yoreland[r - Y_ROW] = title[r] + Y_COL;
		}

		if (r >= T_U_ROW && r < T_U_ROW + T_U_HEIGHT)
		{
			timeUntold[r - T_U_ROW] = title[r] + T_U_COL;
		}
	}

	for (int d = 3; d < 111; d++)
	{
		for (int r = (d >= Y_HEIGHT ? Y_HEIGHT - 1 : d); r >= (d >= Y_WIDTH ? d - Y_WIDTH + 1 : 0); r--)
		{
			mvwaddch(win, r + Y_ROW, d - r + Y_COL, yoreland[r][d - r]);
		}

		wrefresh(win);
		nanosleep(&Y_SLEEP, NULL);
	}

	for (int r = 0; r < T_U_HEIGHT; r++)
	{
		mvwprintw(win, r + T_U_ROW, T_U_COL, "%.*s", T_U_WIDTH, timeUntold[r]);
		wrefresh(win);
		nanosleep(&T_U_SLEEP, NULL);
	}

	for (int r = 0; r < 40; r++)
	{
		free(title[r]);
	}

	free(title);
	free(yoreland);
	free(timeUntold);
	return 0;
}