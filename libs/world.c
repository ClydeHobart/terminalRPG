/* Yoreland: Time Untold | world.c
 *
 * The world module, containing functions related to loading and saving the
 * world.
 *
 * Zeke Baker | 20180705.2323 | terminalRPG/libs/world.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
// #include <time.h>
#include <ncurses.h>
#include "constants.h"
#include "file.h"
#include "player.h"
#include "color.h"

/**************** global types ****************/
typedef struct world {
	WINDOW *worldWin;
	player_t *player;
	char ***chunks;
	char ***depths;
	char ***visited;
} world_t;

/**************** local functions ****************/
static int modulo(int dividend, int divisor);
static int divide(int dividend, int divisor);

/* **************** global functions ****************
 * See world.h for function descriptions
 */

/**************** world_new() ****************/
world_t *
world_new(WINDOW *worldWin, player_t *player)
{
	FILE *mapFile, *mapDepthFile;
	world_t *world = calloc(1, sizeof(world_t));

	if (world == NULL) {
		return NULL;
	}

	world->worldWin = worldWin;
	world->player = player;
	world->chunks = calloc(9, sizeof(char **));
	world->depths = calloc(9, sizeof(char **));
	world->visited = calloc(9, sizeof(char **));

	// if either didn't allocate properly, free as necessary and return NULL
	if (world->chunks == NULL || world->depths == NULL || world->visited == NULL) {
		if (world->chunks == NULL) free(world->chunks);
		if (world->depths == NULL) free(world->depths);
		if (world->visited == NULL) free(world->visited);
		free(world);
		return NULL;
	}

	for (int chunk = 0; chunk < 9; chunk++) {
		mapFile = fopen("assets/maps/testMap1.txt", "r");
		mapDepthFile = fopen("assets/maps/testMap1_depth.txt", "r");
		world->chunks[chunk] = calloc(64, sizeof(char *));
		world->depths[chunk] = calloc(64, sizeof(char *));
		world->visited[chunk] = calloc(64, sizeof(char *));

		// if either didn't allocate properly, free as necessary and return NULL
		if (world->chunks[chunk] == NULL || world->depths[chunk] == NULL || world->visited[chunk] == NULL) {
			if (world->chunks[chunk] == NULL) free(world->chunks[chunk]);
			if (world->depths[chunk] == NULL) free(world->depths[chunk]);
			if (world->visited[chunk] == NULL) free(world->visited[chunk]);

			for (chunk--; chunk >= 0; chunk--) {
				free(world->chunks[chunk]);
				free(world->depths[chunk]);
				free(world->visited[chunk]);
			}

			free(world->chunks);
			free(world->depths);
			free(world->visited);
			free(world);
			return NULL;
		}

		for (int row = 0; row < 64; row++) {
			world->chunks[chunk][row] = readlinep(mapFile);
			world->depths[chunk][row] = readlinep(mapDepthFile);
			world->visited[chunk][row] = calloc(128, sizeof(char));

			// if either didn't allocate properly, free as necessary and return NULL
			if (world->chunks[chunk][row] == NULL || world->depths[chunk][row] == NULL || world->visited[chunk][row] == NULL) {
				if (world->chunks[chunk][row] == NULL) free(world->chunks[chunk][row]);
				if (world->depths[chunk][row] == NULL) free(world->depths[chunk][row]);
				if (world->visited[chunk][row] == NULL) free(world->visited[chunk][row]);

				for (row--; row >= 0; row--) {
					free(world->chunks[chunk][row]);
					free(world->depths[chunk][row]);
					free(world->visited[chunk][row]);
				}

				for (; chunk >= 0; chunk--) {
					free(world->chunks[chunk]);
					free(world->depths[chunk]);
					free(world->visited[chunk]);
				}

				free(world->chunks);
				free(world->depths);
				free(world->visited);
				free(world);
				return NULL;
			}

			for (int col = 0; col < 128; col++) {
				world->visited[chunk][row][col] = '0';
			}
		}

		fclose(mapFile);
		fclose(mapDepthFile);
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
	char ***depths = world->depths;
	char ***visited = world->visited;
	char *cChar, *dChar, *vChar;

	for (int r = 0; r < 27; r++) {
		for (int c = 0; c < 89; c++) {
			if (loc[0] + r - 13 < 0) {
				if (loc[1] + c - 44 < 0) {
					cChar = &(chunks[YTU_CHUNK_NW][loc[0] + r + 51][loc[1] + c + 84]);
					dChar = &(depths[YTU_CHUNK_NW][loc[0] + r + 51][loc[1] + c + 84]);
					vChar = &(visited[YTU_CHUNK_NW][loc[0] + r + 51][loc[1] + c + 84]);
				} else if (loc[1] + c - 44 < 128) {
					cChar = &(chunks[YTU_CHUNK_N][loc[0] + r + 51][loc[1] + c - 44]);
					dChar = &(depths[YTU_CHUNK_N][loc[0] + r + 51][loc[1] + c - 44]);
					vChar = &(visited[YTU_CHUNK_N][loc[0] + r + 51][loc[1] + c - 44]);
				} else {
					cChar = &(chunks[YTU_CHUNK_NE][loc[0] + r + 51][loc[1] + c - 172]);
					dChar = &(depths[YTU_CHUNK_NE][loc[0] + r + 51][loc[1] + c - 172]);
					vChar = &(visited[YTU_CHUNK_NE][loc[0] + r + 51][loc[1] + c - 172]);
				}
			} else if (loc[0] + r - 13 < 64) {
				if (loc[1] + c - 44 < 0) {
					cChar = &(chunks[YTU_CHUNK_W][loc[0] + r - 13][loc[1] + c + 84]);
					dChar = &(depths[YTU_CHUNK_W][loc[0] + r - 13][loc[1] + c + 84]);
					vChar = &(visited[YTU_CHUNK_W][loc[0] + r - 13][loc[1] + c + 84]);
				} else if (loc[1] + c - 44 < 128) {
					cChar = &(chunks[YTU_CHUNK_C][loc[0] + r - 13][loc[1] + c - 44]);
					dChar = &(depths[YTU_CHUNK_C][loc[0] + r - 13][loc[1] + c - 44]);
					vChar = &(visited[YTU_CHUNK_C][loc[0] + r - 13][loc[1] + c - 44]);
				} else {
					cChar = &(chunks[YTU_CHUNK_E][loc[0] + r - 13][loc[1] + c - 172]);
					dChar = &(depths[YTU_CHUNK_E][loc[0] + r - 13][loc[1] + c - 172]);
					vChar = &(visited[YTU_CHUNK_E][loc[0] + r - 13][loc[1] + c - 172]);
				}
			} else {
				if (loc[1] + c - 44 < 0) {
					cChar = &(chunks[YTU_CHUNK_SW][loc[0] + r - 77][loc[1] + c + 84]);
					dChar = &(depths[YTU_CHUNK_SW][loc[0] + r - 77][loc[1] + c + 84]);
					vChar = &(visited[YTU_CHUNK_SW][loc[0] + r - 77][loc[1] + c + 84]);
				} else if (loc[1] + c - 44 < 128) {
					cChar = &(chunks[YTU_CHUNK_S][loc[0] + r - 77][loc[1] + c - 44]);
					dChar = &(depths[YTU_CHUNK_S][loc[0] + r - 77][loc[1] + c - 44]);
					vChar = &(visited[YTU_CHUNK_S][loc[0] + r - 77][loc[1] + c - 44]);
				} else {
					cChar = &(chunks[YTU_CHUNK_SE][loc[0] + r - 77][loc[1] + c - 172]);
					dChar = &(depths[YTU_CHUNK_SE][loc[0] + r - 77][loc[1] + c - 172]);
					vChar = &(visited[YTU_CHUNK_SE][loc[0] + r - 77][loc[1] + c - 172]);
				}
			}

			if (c == 0) {
				wmove(worldWin, r, c);
			}

			if (*vChar != '1' &&
				abs(r - 13) + abs(c - 44) <= 5 * (player_getJUD(world->player) - 1) + 12 &&
				abs(*dChar - depths[YTU_CHUNK_C][loc[0]][loc[1]]) < 5) {
				*vChar = '1';
			}

			if (*vChar == '1' && abs(*dChar - depths[YTU_CHUNK_C][loc[0]][loc[1]]) < 5) {
				wattron(worldWin, COLOR_PAIR(adjustColor(YTU_COLOR_GRAY, *dChar - depths[YTU_CHUNK_C][loc[0]][loc[1]])));
				waddch(worldWin, *cChar);
			} else {
				wattron(worldWin, COLOR_PAIR(YTU_COLOR_GRAY));
				
				if (*vChar == '1') {
					if (*dChar - depths[YTU_CHUNK_C][loc[0]][loc[1]] > 0)
						wattron(worldWin, COLOR_PAIR(adjustColor(YTU_COLOR_GRAY, 4)));
					else
						wattron(worldWin, COLOR_PAIR(adjustColor(YTU_COLOR_GRAY, -4)));
				}
				
				waddch(worldWin, ACS_CKBOARD);
			}
		}
	}

	free(loc);
	wattron(worldWin, COLOR_PAIR(YTU_COLOR_GREEN));
	mvwaddch(worldWin, 13, 44, '@');
	wrefresh(worldWin);
}

/**************** world_handleMove() ****************/
void
world_handleMove(world_t *world, int move)
{
	int *loc = player_getLoc(world->player);
	loc[0] = modulo(loc[0], 64);
	loc[1] = modulo(loc[1], 128);
	char ***depths = world->depths;
	char *dChar;

	switch (move) {
		case KEY_UP:
			dChar = (loc[0] == 0 ? &(depths[YTU_CHUNK_N][63][loc[1]]) :
				&(depths[YTU_CHUNK_C][loc[0] - 1][loc[1]]));
			break;
		case KEY_DOWN:
			dChar = (loc[0] == 63 ? &(depths[YTU_CHUNK_S][0][loc[1]]) :
				&(depths[YTU_CHUNK_C][loc[0] + 1][loc[1]]));
			break;
		case KEY_LEFT:
			dChar = (loc[1] == 0 ? &(depths[YTU_CHUNK_W][loc[0]][127]) :
				&(depths[YTU_CHUNK_C][loc[0]][loc[1] - 1]));
			break;
		case KEY_RIGHT:
			dChar = (loc[1] == 127 ? &(depths[YTU_CHUNK_E][loc[0]][0]) :
				&(depths[YTU_CHUNK_C][loc[0]][loc[1] + 1]));
			break;
	}

	if (abs(*dChar - depths[YTU_CHUNK_C][loc[0]][loc[1]]) < 2) {
		player_move(world->player, move);
	}

	world_print(world);
	free(loc);
}

/**************** world_delete() ****************/
void
world_delete(world_t *world)
{
	for (int c = 0; c < 9; c++) {
		for (int r = 0; r < 64; r++) {
			free(world->chunks[c][r]);
			free(world->depths[c][r]);
			free(world->visited[c][r]);
		}

		free(world->chunks[c]);
		free(world->depths[c]);
		free(world->visited[c]);
	}

	free(world);
}

/**************** world_save() ****************/
void
world_save(world_t *world)
{
	FILE *vFile;
	int *loc;
	char *vFileName;
	char *playerName;

	loc = player_getLoc(world->player);
	loc[0] = divide(loc[0], 64);
	loc[1] = divide(loc[1], 128);
	playerName = player_getName(world->player);
	vFileName = calloc(strlen(playerName) + 21, sizeof(char));
	sprintf(vFileName, "saves/%s", playerName);
	mkdir(vFileName, 00777);

	for (int chRow = 0; chRow < 3; chRow++) {
		for (int chCol = 0; chCol < 3; chCol++) {
			sprintf(vFileName, "saves/%s/%.3d_%.3d.txt", playerName, loc[0] + chRow + 127, loc[1] + chCol + 127);
			vFile = fopen(vFileName, "w");

			if (vFile != NULL) {
				for (int r = 0; r < 64; r++) {
					fprintf(vFile, "%s\n", world->visited[chRow * 3 + chCol][r]);
				}
			}

			fclose(vFile);
		}
	}

	free(vFileName);
	free(playerName);
	free(loc);
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
static int
modulo(int dividend, int divisor)
{
	int remainder = dividend % divisor;
	return (remainder < 0 ? remainder + divisor : remainder);
}

/* **************** divide() ****************
 * Performs division that returns the quotient via floored division
 *
 * dividend: int of number to perform division on
 * divisor: int of number to perform division by
 * return: floored division quotient of dividend / divisor
 */
static int
divide(int dividend, int divisor)
{
	return (dividend < 0 ? dividend / divisor - 1 : dividend / divisor);
}