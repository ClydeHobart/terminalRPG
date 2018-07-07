/* Yoreland: Time Untold | yoreland.c
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
#include "libs/player.h"
#include "libs/world.h"

int main(void)
{
	WINDOW *gameWin;	// WINDOW * for the game
	WINDOW *worldWin;	// WINDOW * for the world
	player_t *player;	// player_t * for the player
	world_t *world;
	int currErr;			// int for current exit code error number
	int height;				// int for height of stdscr
	int width;				// int for width of stdscr
	int response;			// int for function responses

	currErr = 1;
	initscr();
	noecho();
	curs_set(0);

	if (getmaxyx(stdscr, height, width), height < 40 || width < 120) {
		mvprintw(height / 2, (width / 2 - 25 < 0 ? 0 : width / 2 - 25),
			"The window must be at least 40 rows by 120 columns.");
		mvprintw(height / 2 + 1, (width / 2 - 11 < 0 ? 0 : width / 2 - 11),
			"Press any key to quit.");
		refresh();
		getch();
		endwin();
		return currErr;
	}

	currErr++;
	gameWin = newwin(40, 120, height / 2 - 20, width / 2 - 60);
	refresh();
	response = openingTitle_anim(gameWin);

	if (response) {
		endwin();
		return currErr;
	}

	currErr++;
	getch();
	wclear(gameWin);
	wborder(gameWin, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
		'O', 'O', 'O', 'O');
	wrefresh(gameWin);
	worldWin = derwin(gameWin, 27, 89, 1, 30);
	player = player_new();
	world = world_new(player);
	world_print(worldWin, world);
	wrefresh(worldWin);
	getch();
	endwin();
	return 0;
}