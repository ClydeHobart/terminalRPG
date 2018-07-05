/* yoreland.c
 * The main .c file for Yoreland: Time Untold.
 *
 * Zeke Baker | 20180702.2215 | terminalRPG/yoreland.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "libs/animations.h"

int main(void)
{
	WINDOW *gameWin;	// WINDOW * for the game
	int height;				// int for height of stdscr
	int width;				// int for width of stdscr
	int response;			// int for function responses

	initscr();
	curs_set(0);

	if (getmaxyx(stdscr, height, width), height < 40 || width < 120)
	{
		mvprintw(height / 2, (width / 2 - 25 < 0 ? 0 : width / 2 - 25),
			"The window must be at least 40 rows by 120 columns.");
		mvprintw(height / 2 + 1, (width / 2 - 11 < 0 ? 0 : width / 2 - 11),
			"Press any key to quit.");
		refresh();
		getch();
		endwin();
		return 1;
	}

	gameWin = newwin(40, 120, height / 2 - 20, width / 2 - 60);
	refresh();
	response = openingTitle_anim(gameWin);


	if (response)
	{
		endwin();
		return 2;
	}

	getch();
	endwin();
	return 0;
}