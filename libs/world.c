/* Yoreland: Time Untold | world.c
 *
 * The world module, containing functions related to loading and saving the
 * world.
 *
 * Zeke Baker | 20180705.2323 | terminalRPG/libs/world.c
 */

#include <stdio.h>
#include <stdlib.h>
// #include <time.h>
#include <ncurses.h>
#include "file.h"
#include "player.h"

/**************** constants ****************/
static const int CHUNK_C = 0;
static const int CHUNK_N = 1;
static const int CHUNK_NE = 2;
static const int CHUNK_E = 3;
static const int CHUNK_SE = 4;
static const int CHUNK_S = 5;
static const int CHUNK_SW = 6;
static const int CHUNK_W = 7;
static const int CHUNK_NW = 8;

/**************** global types ****************/
typedef struct world {
	WINDOW *worldWin;
	player_t *player;
	char **chunks[9];
} world_t;

/**************** local functions ****************/
static int modulo(int dividend, int divisor);

/* **************** global functions ****************
 * See world.h for function descriptions
 */

/**************** world_new() ****************/
world_t *
world_new(WINDOW *worldWin, player_t *player)
{
	FILE *file;
	world_t *world = calloc(1, sizeof(world_t));

	if (world == NULL) {
		return world;
	}

	world->worldWin = worldWin;
	world->player = player;

	for (int c = 0; c < 9; c++) {
		file = fopen("assets/maps/testMap1.txt", "r");
		world->chunks[c] = calloc(64, sizeof(char *));

		for (int r = 0; r < 64; r++) {
			world->chunks[c][r] = readlinep(file);
		}

		fclose(file);
	}

	return world;
}

/**************** world_print() ****************/
void
world_print(world_t *world)
{
	WINDOW *worldWin = world->worldWin;
	int *loc = player_getLoc(world->player);
	loc[0] = modulo(loc[0], 64);
	loc[1] = modulo(loc[1], 128);
	char ***chunks = world->chunks;

	for (int r = 0; r < 27; r++) {
		if (loc[0] + r - 13 < 0) {
			if (loc[1] - 44 < 0) {
				mvwprintw(worldWin, r, 0, "%.*s", 44 - loc[1], chunks[CHUNK_NW][loc[0] + r + 51] + loc[1] + 84);
				wprintw(worldWin, "%.*s", loc[1] + 45, chunks[CHUNK_N][loc[0] + r + 51]);
			} else if (loc[1] + 44 < 128) {
				mvwprintw(worldWin, r, 0, "%.89s", chunks[CHUNK_N][loc[0] + r + 51] + loc[1] - 44);
			} else {
				mvwprintw(worldWin, r, 0, "%.*s", 172 - loc[1], chunks[CHUNK_N][loc[0] + r + 51] + loc[1] - 44);
				wprintw(worldWin, "%.*s", loc[1] - 83, chunks[CHUNK_NE][loc[0] + r + 51]);
			}
		} else if (loc[0] + r - 13 < 64) {
			if (loc[1] - 44 < 0) {
				mvwprintw(worldWin, r, 0, "%.*s", 44 - loc[1], chunks[CHUNK_W][loc[0] + r - 13] + loc[1] + 84);
				wprintw(worldWin, "%.*s", loc[1] + 45, chunks[CHUNK_C][loc[0] + r - 13]);
			} else if (loc[1] + 44 < 128) {
				mvwprintw(worldWin, r, 0, "%.89s", chunks[CHUNK_C][loc[0] + r - 13] + loc[1] - 44);
			} else {
				mvwprintw(worldWin, r, 0, "%.*s", 172 - loc[1], chunks[CHUNK_C][loc[0] + r - 13] + loc[1] - 44);
				wprintw(worldWin, "%.*s", loc[1] - 83, chunks[CHUNK_E][loc[0] + r - 13]);
			}
		} else {
			if (loc[1] - 44 < 0) {
				mvwprintw(worldWin, r, 0, "%.*s", 44 - loc[1], chunks[CHUNK_SW][loc[0] + r - 77] + loc[1] + 84);
				wprintw(worldWin, "%.*s", loc[1] + 45, chunks[CHUNK_S][loc[0] + r - 77]);
			} else if (loc[1] + 44 < 128) {
				mvwprintw(worldWin, r, 0, "%.89s", chunks[CHUNK_S][loc[0] + r - 77] + loc[1] - 44);
			} else {
				mvwprintw(worldWin, r, 0, "%.*s", 172 - loc[1], chunks[CHUNK_S][loc[0] + r - 77] + loc[1] - 44);
				wprintw(worldWin, "%.*s", loc[1] - 83, chunks[CHUNK_SE][loc[0] + r - 77]);
			}
		}
	}

	mvwaddch(worldWin, 13, 44, '@');
	wrefresh(worldWin);
}

/**************** world_handleMove() ****************/
void
world_handleMove(world_t *world, int move)
{
	player_move(world->player, move);
	world_print(world);
}

/* **************** modulo() ****************
 * Performs modulus division that returns the remainder via floored division
 * instead of truncated division 
 * (see https://en.wikipedia.org/wiki/Modulo_operation#/media/File:Divmod.svg)
 *
 * dividend: int of number to perform modulo division on
 * divisor: int of number to perform modulo division by
 * return: floored-division remainder of dividend % divisor
 */
static int modulo(int dividend, int divisor)
{
	int remainder = dividend % divisor;
	return (remainder < 0 ? remainder + divisor : remainder);
}