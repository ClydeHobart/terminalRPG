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
#include <stdbool.h>
#include <sys/stat.h>
#include <ncurses.h>
#include "constants.h"
#include "file.h"
#include "player.h"
#include "color.h"

/**************** global types ****************/
typedef struct world {
	WINDOW *win;
	player_t *player;
	char chunks[9][64][129];
	char depths[9][64][129];
	char visited[9][64][129];
} world_t;

/**************** local functions ****************/
static int modulo(const int dividend, const int divisor);
static int divide(const int dividend, const int divisor);
static bool world_saveChunk(world_t *world, const int chunk);
static bool world_loadChunk(world_t *world, const int chunk);

/* **************** global functions ****************
 * See world.h for function descriptions
 */
world_t *world_new(WINDOW *win, player_t *player);
void world_print(world_t *world);
void world_handleMove(world_t *world, int move);
bool world_save(world_t *world);
world_t *world_load(WINDOW *win, player_t *player);

/**************** world_new() ****************/
world_t *
world_new(WINDOW *win, player_t *player)
{
	FILE *mapFile, *mapDepthFile;
	world_t *world = calloc(1, sizeof(world_t));
	char *line;

	if (world == NULL) {
		return NULL;
	}

	world->win = win;
	world->player = player;

	for (int chunk = 0; chunk < 9; chunk++) {
		mapFile = fopen("assets/maps/testMap1.txt", "r");
		mapDepthFile = fopen("assets/maps/testMap1_depth.txt", "r");

		for (int row = 0; row < 64; row++) {
			line = readlinep(mapFile);
			strncpy(world->chunks[chunk][row], line, 128);
			free(line);
			line = readlinep(mapDepthFile);
			strncpy(world->depths[chunk][row], line, 128);
			free(line);

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
	int *loc;			// int array for player location
	char *dChar;	// char pointer for depth of desired move location

	loc = player_getLoc(world->player);
	loc[0] = modulo(loc[0], 64);
	loc[1] = modulo(loc[1], 128);

	switch (move) {
		case KEY_UP:
			dChar = (loc[0] == 0 ? &(world->depths[YTU_CHUNK_N][63][loc[1]]) :
				&(world->depths[YTU_CHUNK_C][loc[0] - 1][loc[1]]));
			break;
		case KEY_DOWN:
			dChar = (loc[0] == 63 ? &(world->depths[YTU_CHUNK_S][0][loc[1]]) :
				&(world->depths[YTU_CHUNK_C][loc[0] + 1][loc[1]]));
			break;
		case KEY_LEFT:
			dChar = (loc[1] == 0 ? &(world->depths[YTU_CHUNK_W][loc[0]][127]) :
				&(world->depths[YTU_CHUNK_C][loc[0]][loc[1] - 1]));
			break;
		case KEY_RIGHT:
			dChar = (loc[1] == 127 ? &(world->depths[YTU_CHUNK_E][loc[0]][0]) :
				&(world->depths[YTU_CHUNK_C][loc[0]][loc[1] + 1]));
			break;
	}

	if (abs(*dChar - world->depths[YTU_CHUNK_C][loc[0]][loc[1]]) < 2) {
		player_move(world->player, move);
	}

	world_print(world);
	free(loc);
}

/**************** world_save() ****************/
bool
world_save(world_t *world)
{
	char *vDirName;		// string for visited directory name
	char *playerName;	// string for player name
	bool worldSaved;	// bool for whether world saved properly

	worldSaved = true;

	do {
		playerName = player_getName(world->player);

		if (!playerName) {
			worldSaved = false;
			break;
		}

		vDirName = calloc(strlen(playerName) + 7, sizeof(char));

		if (!vDirName) {
			worldSaved = false;
			break;
		}

		sprintf(vDirName, "saves/%s", playerName);
		mkdir(vDirName, 00777);

		for (int c = 0; c < 9; c++) {
			if (!world_saveChunk(world, c)) {
				worldSaved = false;
				break;
			}
		}
	} while (false);

	if (vDirName) free(vDirName);
	if (playerName) free(playerName);
	return worldSaved;
}

/**************** world_load() ****************/
world_t *
world_load(WINDOW *win, player_t *player)
{
	world_t *world;	// world_t pointer for world to load

	do {
		world = calloc(1, sizeof(world_t));
		
		if (!world) break;
		
		world->win = win;
		world->player = player;

		for (int c = 0; c < 9; c++) {
			if (!world_loadChunk(world, c)) {
				free(world);
				return NULL;
			}
		}
	} while (false);

	return world;
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
 * return: bool of whether chunk saved properly
 */
static bool
world_saveChunk(world_t *world, const int chunk)
{
	FILE *vFile;			// FILE pointer for world->visited
	int *loc;					// int array for player location
	char *vFileName;	// string for visted file name
	char *playerName;	// string for player name
	bool chunkSaved;	// bool for whether chunk saved properly

	chunkSaved = true;

	do {
		loc = player_getLoc(world->player);

		if (!loc) {
			chunkSaved = false;
			break;
		}

		loc[0] = divide(loc[0], 64);
		loc[1] = divide(loc[1], 128);
		playerName = player_getName(world->player);

		if (!playerName) {
			chunkSaved = false;
			break;
		}

		vFileName = calloc(strlen(playerName) + 19, sizeof(char));

		if (!vFileName) {
			chunkSaved = false;
			break;
		}

		sprintf(vFileName, "saves/%s/%.3d_%.3d.txt", playerName,
			loc[0] + (chunk / 3) + 127,
			loc[1] + (chunk % 3) + 127);
		vFile = fopen(vFileName, "w");

		if (vFile) {
			for (int row = 0; row < 64; row++) {
				fprintf(vFile, "%s\n", world->visited[chunk][row]);
			}
			
			fclose(vFile);
		} else chunkSaved = false;
	} while (false);

	if (vFileName) free(vFileName);
	if (playerName) free(playerName);
	if (loc) free(loc);
	return chunkSaved;
}

/* **************** world_loadChunk() ****************
 * Loads a specific chunk of a world
 *
 * world: world_t pointer of world to load the chunk of
 * chunk: int of index of chunk to load
 * return: bool for if chunk loaded properly
 */
static bool
world_loadChunk(world_t *world, const int chunk)
{
	FILE *cFile;			// FILE pointer for world->chunks
	FILE *dFile;			// FILE pointer for world->depths
	FILE *vFile;			// FILE pointer for world->visited
	int *loc;					// int array for player location
	char *vFileName;	// string for visited file name
	char *playerName;	// string for player name
	char *line;				// string for line read from file
	bool chunkLoaded;	// bool for whether the chunk loaded properly

	chunkLoaded = true;

	do {
		loc = player_getLoc(world->player);

		if (!loc) {
			chunkLoaded = false;
			break;
		}

		loc[0] = divide(loc[0], 64);
		loc[1] = divide(loc[1], 128);
		playerName = player_getName(world->player);

		if (!playerName) {
			chunkLoaded = false;
			break;
		}

		vFileName = calloc(strlen(playerName) + 19, sizeof(char));

		if (!vFileName) {
			chunkLoaded = false;
			break;
		}

		sprintf(vFileName, "saves/%s/%.3d_%.3d.txt", playerName,
			loc[0] + (chunk / 3) + 127,
			loc[1] + (chunk % 3) + 127);
		vFile = fopen(vFileName, "r");

		cFile = fopen("assets/maps/testMap1.txt", "r");
		dFile = fopen("assets/maps/testMap1_depth.txt", "r");

		if (cFile && dFile && vFile) {
			for (int row = 0; row < 64; row++) {
				line = readlinep(cFile);

				if (!line) {
					chunkLoaded = false;
					break;
				}

				strcpy(world->chunks[chunk][row], line);
				free(line);
				line = readlinep(dFile);

				if (!line) {
					chunkLoaded = false;
					break;
				}

				strcpy(world->depths[chunk][row], line);
				free(line);
				line = readlinep(vFile);

				if (!line) {
					chunkLoaded = false;
					break;
				}

				strcpy(world->visited[chunk][row], line);
				free(line);
			}
		} else chunkLoaded = false;
			
		if (cFile) fclose(cFile);
		if (dFile) fclose(dFile);
		if (vFile) fclose(vFile);
	} while (false);

	if (vFileName) free(vFileName);
	if (playerName) free(playerName);
	if (loc) free(loc);
	return chunkLoaded;
}