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
#include "libs/color.h"

int main(const int argc, char *argv[])
{
	WINDOW *gameWin;	// WINDOW * for the game
	WINDOW *worldWin;	// WINDOW * for the world
	player_t *player;	// player_t * for the player
	world_t *world;
	int *loc;					// DEBUG
	int currErr;			// int for current exit code error number
	int height;				// int for height of stdscr
	int width;				// int for width of stdscr
	int response;			// int for function responses
	int key;					// int for key presses
	short ***initials;

	currErr = 1;
	initscr();
	noecho();
	curs_set(0);
	start_color();


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

	if (argc < 2 || strcmp("false", argv[1])) {
		response = openingTitle_anim(gameWin);

		if (response) {
			endwin();
			return currErr;
		}
	} else {
		mvprintw(height / 2, (width / 2 - 11 < 0 ? 0 : width / 2 - 13),
			"Press any key to continue.");
	}

	keypad(gameWin, true);
	currErr++;
	getch();
	wclear(gameWin);
	wborder(gameWin, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
		'O', 'O', 'O', 'O');
	wrefresh(gameWin);
	initials = initColors();
	worldWin = derwin(gameWin, 27, 89, 1, 30);
	player = player_load("Greg");
	world = world_new(worldWin, player);
	world_print(world);
	key = wgetch(gameWin);

	while ( key != 27 ) {
		if ( key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT ) {
			world_handleMove(world, key);
		}

		// BEG DEBUG
		loc = player_getLoc(player);
		mvprintw(0, 0, "row: %8d", loc[0]);
		mvprintw(1, 0, "col: %8d", loc[1]);
		refresh();
		// END DEBUG

		key = wgetch(gameWin);
	}

	world_save(world);
	world_delete(world);
	player_save(player);
	resetColors(initials);
	endwin();
	return 0;
}