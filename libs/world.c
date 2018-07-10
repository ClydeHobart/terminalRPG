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
	char ***chunks;
	char ***visited;
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
		return NULL;
	}

	world->worldWin = worldWin;
	world->player = player;
	world->chunks = calloc(9, sizeof(char **));
	world->visited = calloc(9, sizeof(char **));

	// if either didn't allocate properly, free as necessary and return NULL
	if (world->chunks == NULL || world->visited == NULL) {
		if (world->chunks == NULL) free(world->chunks);
		if (world->visited == NULL) free(world->visited);
		free(world);
		return NULL;
	}

	for (int c = 0; c < 9; c++) {
		file = fopen("assets/maps/testMap1.txt", "r");
		world->chunks[c] = calloc(64, sizeof(char *));
		world->visited[c] = calloc(64, sizeof(char *));

		// if either didn't allocate properly, free as necessary and return NULL
		if (world->chunks[c] == NULL || world->visited[c] == NULL) {
			if (world->chunks[c] == NULL) free(world->chunks[c]);
			if (world->visited[c] == NULL) free(world->visited[c]);

			for (c--; c >= 0; c--) {
				free(world->chunks[c]);
				free(world->visited[c]);
			}

			free(world->chunks);
			free(world->visited);
			free(world);
			return NULL;
		}

		for (int r = 0; r < 64; r++) {
			world->chunks[c][r] = readlinep(file);
			world->visited[c][r] = calloc(128, sizeof(char));

			// if either didn't allocate properly, free as necessary and return NULL
			if (world->chunks[c][r] == NULL || world->visited[c][r] == NULL) {
				if (world->chunks[c][r] == NULL) free(world->chunks[c][r]);
				if (world->visited[c][r] == NULL) free(world->visited[c][r]);

				for (r--; r >= 0; r--) {
					free(world->chunks[c][r]);
					free(world->visited[c][r]);
				}

				for (; c >= 0; c--) {
					free(world->chunks[c]);
					free(world->visited[c]);
				}

				free(world->chunks);
				free(world->visited);
				free(world);
				return NULL;
			}
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
	// int chunk, row, col;
	loc[0] = modulo(loc[0], 64);
	loc[1] = modulo(loc[1], 128);
	char ***chunks = world->chunks;
	char ***visited = world->visited;
	char *cChar, *vChar;

	for (int r = 0; r < 27; r++) {
		for (int c = 0; c < 89; c++) {
			if (loc[0] + r - 13 < 0) {
				if (loc[1] + c - 44 < 0) {
					cChar = &(chunks[CHUNK_NW][loc[0] + r + 51][loc[1] + c + 84]);
					vChar = &(visited[CHUNK_NW][loc[0] + r + 51][loc[1] + c + 84]);
				} else if (loc[1] + c - 44 < 128) {
					cChar = &(chunks[CHUNK_N][loc[0] + r + 51][loc[1] + c - 44]);
					vChar = &(visited[CHUNK_N][loc[0] + r + 51][loc[1] + c - 44]);
				} else {
					cChar = &(chunks[CHUNK_NE][loc[0] + r + 51][loc[1] + c - 172]);
					vChar = &(visited[CHUNK_NE][loc[0] + r + 51][loc[1] + c - 172]);
				}
			} else if (loc[0] + r - 13 < 64) {
				if (loc[1] + c - 44 < 0) {
					cChar = &(chunks[CHUNK_W][loc[0] + r - 13][loc[1] + c + 84]);
					vChar = &(visited[CHUNK_W][loc[0] + r - 13][loc[1] + c + 84]);
				} else if (loc[1] + c - 44 < 128) {
					cChar = &(chunks[CHUNK_C][loc[0] + r - 13][loc[1] + c - 44]);
					vChar = &(visited[CHUNK_C][loc[0] + r - 13][loc[1] + c - 44]);
				} else {
					cChar = &(chunks[CHUNK_E][loc[0] + r - 13][loc[1] + c - 172]);
					vChar = &(visited[CHUNK_E][loc[0] + r - 13][loc[1] + c - 172]);
				}
			} else {
				if (loc[1] + c - 44 < 0) {
					cChar = &(chunks[CHUNK_SW][loc[0] + r - 77][loc[1] + c + 84]);
					vChar = &(visited[CHUNK_SW][loc[0] + r - 77][loc[1] + c + 84]);
				} else if (loc[1] + c - 44 < 128) {
					cChar = &(chunks[CHUNK_S][loc[0] + r - 77][loc[1] + c - 44]);
					vChar = &(visited[CHUNK_S][loc[0] + r - 77][loc[1] + c - 44]);
				} else {
					cChar = &(chunks[CHUNK_SE][loc[0] + r - 77][loc[1] + c - 172]);
					vChar = &(visited[CHUNK_SE][loc[0] + r - 77][loc[1] + c - 172]);
				}
			}

			if (c == 0) {
				wmove(worldWin, r, c);
			}

			if (*vChar == 1) {
				waddch(worldWin, *cChar);
			} else if (abs(r - 13) + abs(c - 44) <= 5 * (player_getJUD(world->player) - 1) + 12 ) {
				*vChar = 1;
				waddch(worldWin, *cChar);
			} else {
				waddch(worldWin, ACS_CKBOARD);
			}
		}
	}

	free(loc);
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