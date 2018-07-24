/* Yoreland: Time Untold | player.c
 * 
 * The player module, containing the player struct and its related functions
 *
 * Zeke Baker | 20180705.0011 | terminalRPG/libs/player.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "file.h"
// #include "inventory.h"

const int STAT_MIG = 0;
const int STAT_VIG = 1;
const int STAT_LEG = 2;
const int STAT_INT = 3;
const int STAT_JUD = 4;
const int STAT_CHA = 5;
const int STAT_FAV = 6;

/**************** global types ****************/
typedef struct player {
	// inventory_t *inventory;
	int stats[7];
	int statMods[7];
	int lvl;
	int exp;
	int vitality;
	int maxVitality;
	int stamina;
	int maxStamina;
	int burden;
	int maxBurden;
	int row;
	int col;
	char name[20];
} player_t;

/* **************** global functions ****************
 * see player.h for comments about exported functions
 */

/**************** player_new() ****************/
player_t *
player_new(const char *name)
{
	player_t *player = calloc(1, sizeof(player_t));

	if (player == NULL) {
		return NULL;
	}

	strncpy(player->name, name, 20);

	for (int s = 0; s < 7; s++) {
		player->stats[s] = 1;
	}

	player->lvl = 1;
	player->exp = 0;
	player->vitality = 100;
	player->maxVitality = 100;
	player->stamina = 100;
	player->maxStamina = 100;
	player->burden = 100;
	player->maxBurden = 100;
	player->row = 32;
	player->col = 64;
	return player;
}

/**************** player_getLoc() ****************/
int *
player_getLoc(player_t *player)
{
	int *loc = calloc(2, sizeof(int));

	if (loc != NULL) {
		loc[0] = player->row;
		loc[1] = player->col;
	}

	return loc;
}

/**************** player_getStats() ****************/
int*
player_getStats(player_t *player)
{
	int *stats = calloc(7, sizeof(int));

	if (stats != NULL) {
		for (int s = 0; s < 7; s++) {
			stats[s] = player->stats[s];
		}
	}

	return stats;
}

/**************** player_getMIG() ****************/
int
player_getMIG(player_t *player) {return player->stats[STAT_MIG];}

/**************** player_getVIG() ****************/
int
player_getVIG(player_t *player) {return player->stats[STAT_VIG];}

/**************** player_getLEG() ****************/
int
player_getLEG(player_t *player) {return player->stats[STAT_LEG];}

/**************** player_getINT() ****************/
int
player_getINT(player_t *player) {return player->stats[STAT_INT];}

/**************** player_getJUD() ****************/
int
player_getJUD(player_t *player) {return player->stats[STAT_JUD];}

/**************** player_getCHA() ****************/
int
player_getCHA(player_t *player) {return player->stats[STAT_CHA];}

/**************** player_getFAV() ****************/
int
player_getFAV(player_t *player) {return player->stats[STAT_FAV];}

/**************** player_getName() ****************/
char *
player_getName(player_t *player)
{
	char *name;	// string for player name
	name = calloc(strlen(player->name) + 1, sizeof(char));
	return strcpy(name, player->name);
}

/**************** player_move() ****************/
void
player_move(player_t *player, int move)
{
	switch (move) {
		case KEY_UP:
			player->row--;
			// if (player->row == -1) player->row = 63;
			break;
		case KEY_DOWN:
			player->row++;
			// if (player->row == 64) player->row = 0;
			break;
		case KEY_LEFT:
			player->col--;
			// if (player->col == -1) player->col = 127;
			break;
		case KEY_RIGHT:
			player->col++;
			// if (player->col == 128) player->col = 0;
			break;
	}
}

/**************** player_save() ****************/
void
player_save(player_t *player)
{
	FILE *pFile;			// FILE pointer to player save file
	char *pFileName;	// string for player save file name

	pFileName = calloc(strlen(player->name) + 18, sizeof(char));

	if (pFileName == NULL) return;

	sprintf(pFileName, "saves/%s/player.txt", player->name);
	pFile = fopen(pFileName, "w");

	if (pFile != NULL) {
		fprintf(pFile, "%s\n", player->name);
		fprintf(pFile, "Stats:      ");

		for (int s = 0; s < 7; s++) {
			fprintf(pFile, "%d%s", player->stats[s], s == 6 ? "\n" : ", ");
		}

		fprintf(pFile, "Stat mods:  ");

		for (int s = 0; s < 7; s++) {
			fprintf(pFile, "%d%s", player->statMods[s], s == 6 ? "\n" : ", ");
		}

		fprintf(pFile, "Level:      %d\n", player->lvl);
		fprintf(pFile, "Experience: %d\n", player->exp);
		fprintf(pFile, "Vitality:   %.3d / %.3d\n", player->vitality, player->maxVitality);
		fprintf(pFile, "Stamina:    %.3d / %.3d\n", player->stamina, player->maxStamina);
		fprintf(pFile, "Burden:     %.3d / %.3d\n", player->burden, player->maxBurden);
		fprintf(pFile, "Location:   (%d, %d)", player->row, player->col);
	}

	fclose(pFile);
	free(pFileName);
}

/**************** player_load() ****************/
player_t *
player_load(const char *name)
{
	FILE *pFile;
	player_t *player;
	char *pFileName;
	char *line;

	do {
		pFileName = calloc(strlen(name) + 18, sizeof(char));
		sprintf(pFileName, "saves/%s/player.txt", name);
		pFile = fopen(pFileName, "r");
		player = calloc(1, sizeof(player_t));

		if (!pFileName || !pFile || !player) break;

		strcpy(player->name, name);
		line = readlinep(pFile);
		free(line);
		line = readlinep(pFile);
		sscanf(line, "Stats:      %d, %d, %d, %d, %d, %d, %d", &(player->stats[0]),
			&(player->stats[1]), &(player->stats[2]), &(player->stats[3]),
			&(player->stats[4]), &(player->stats[5]), &(player->stats[6]));
		free(line);
		line = readlinep(pFile);
		sscanf(line, "Stat mods:  %d, %d, %d, %d, %d, %d, %d", &(player->statMods[0]),
			&(player->statMods[1]), &(player->statMods[2]), &(player->statMods[3]),
			&(player->statMods[4]), &(player->statMods[5]), &(player->statMods[6]));
		free(line);
		line = readlinep(pFile);
		sscanf(line, "Level:      %d", &(player->lvl));
		free(line);
		line = readlinep(pFile);
		sscanf(line, "Experience: %d", &(player->exp));
		free(line);
		line = readlinep(pFile);
		sscanf(line, "Vitality:   %d / %d", &(player->vitality), &(player->maxVitality));
		free(line);
		line = readlinep(pFile);
		sscanf(line, "Stamina:    %d / %d", &(player->stamina), &(player->maxStamina));
		free(line);
		line = readlinep(pFile);
		sscanf(line, "Burden:     %d / %d", &(player->burden), &(player->maxBurden));
		free(line);
		line = readlinep(pFile);
		sscanf(line, "Location:   (%d, %d)", &(player->row), &(player->col));
		free(line);
	} while (false);

	if (pFileName) free(pFileName);
	if (pFile) fclose(pFile);
	return player;
}