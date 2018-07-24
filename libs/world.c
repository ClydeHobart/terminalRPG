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
	WINDOW *win;
	player_t *player;
	char ***chunks;
	char ***depths;
	char ***visited;
} world_t;

/**************** local functions ****************/
static int modulo(const int dividend, const int divisor);
static int divide(const int dividend, const int divisor);
static void world_saveChunk(world_t *world, const int chunk);

/* **************** global functions ****************
 * See world.h for function descriptions
 */

/**************** world_new() ****************/
world_t *
world_new(WINDOW *win, player_t *player)
{
	FILE *mapFile, *mapDepthFile;
	world_t *world = calloc(1, sizeof(world_t));

	if (world == NULL) {
		return NULL;
	}

	world->win = win;
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
	WINDOW *win;		// WINDOW pointer of the world
	int *loc;				// int array of the player's location
	int chunk;			// int of the chunk of the print cell
	int chRow;			// int of the row relative to the chunk of the print cell
	int chCol;			// int of the col relative to the chunk of the print cell
	int depthDiff;	// int for the difference in depth of the print cell and the player cell
	char *cChar;		// char pointer for the chunks value of the print cell
	char *dChar;		// char pointer for the depths value of the print cell
	char *vChar;		// char pointer for the visited value of the print cell

	win = world->win;
	loc = player_getLoc(world->player);
	loc[0] = modulo(loc[0], 64);
	loc[1] = modulo(loc[1], 128);

	for (int winRow = 0; winRow < 27; winRow++) {
		for (int winCol = 0; winCol < 89; winCol++) {
			chunk = (divide(loc[0] + winRow - 13, 64) + 1) * 3 + divide(loc[1] + winCol - 44, 128) + 1;
			chRow = loc[0] + winRow - 13 + 64 * (1 - (chunk / 3));
			chCol = loc[1] + winCol - 44 + 128 * (1 - (chunk % 3));
			cChar = &(world->chunks[chunk][chRow][chCol]);
			dChar = &(world->depths[chunk][chRow][chCol]);
			vChar = &(world->visited[chunk][chRow][chCol]);
			depthDiff = *dChar - world->depths[YTU_CHUNK_C][loc[0]][loc[1]];

			if (winCol == 0) {
				wmove(win, winRow, winCol);
			}

			if (*vChar != '1' &&
				abs(winRow - 13) + abs(winCol - 44) <= 5 * (player_getJUD(world->player) - 1) + 12 &&
				abs(depthDiff) < 5) {
				*vChar = '1';
			}

			if (*vChar == '1' && abs(depthDiff) < 5) {
				wattron(win, COLOR_PAIR(adjustColor(YTU_COLOR_GRAY, depthDiff)));
				waddch(win, *cChar);
			} else {
				wattron(win, COLOR_PAIR(YTU_COLOR_GRAY));
				
				if (*vChar == '1') {
					if (depthDiff > 0)
						wattron(win, COLOR_PAIR(adjustColor(YTU_COLOR_GRAY, 4)));
					else
						wattron(win, COLOR_PAIR(adjustColor(YTU_COLOR_GRAY, -4)));
				}
				
				waddch(win, ACS_CKBOARD);
			}
		}
	}

	free(loc);
	wattron(win, COLOR_PAIR(YTU_COLOR_GREEN));
	mvwaddch(win, 13, 44, '@');
	wrefresh(win);
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
	char *vDirName;
	char *playerName;

	playerName = player_getName(world->player);
	vDirName = calloc(strlen(playerName) + 7, sizeof(char));
	sprintf(vDirName, "saves/%s", playerName);
	mkdir(vDirName, 00777);
	free(vDirName);
	free(playerName);

	for (int c = 0; c < 9; c++) {
		world_saveChunk(world, c);
	}
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
modulo(const int dividend, const int divisor)
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
divide(const int dividend, const int divisor)
{
	return (dividend < 0 ? dividend / divisor - 1 : dividend / divisor);
}

/* **************** world_saveChunk() ****************
 * Saves a specific chunk of a world
 *
 * world: world_t pointer of world to save the chunk of
 * chunk: int of index of chunk to save
 */
static void
world_saveChunk(world_t *world, const int chunk)
{
	FILE *vFile;
	int *loc;
	int chRow;
	int chCol;
	char *vFileName;
	char *playerName;

	loc = player_getLoc(world->player);
	loc[0] = divide(loc[0], 64);
	loc[1] = divide(loc[1], 128);
	chRow = chunk / 3;
	chCol = chunk % 3;
	playerName = player_getName(world->player);
	vFileName = calloc(strlen(playerName) + 19, sizeof(char));
	sprintf(vFileName, "saves/%s/%.3d_%.3d.txt", playerName, loc[0] + chRow + 127, loc[1] + chCol + 127);
	vFile = fopen(vFileName, "w");

	if (vFile != NULL) {
		for (int r = 0; r < 64; r++) {
			fprintf(vFile, "%s\n", world->visited[chRow * 3 + chCol][r]);
		}
		
		fclose(vFile);
	}

	free(vFileName);
	free(playerName);
	free(loc);
}