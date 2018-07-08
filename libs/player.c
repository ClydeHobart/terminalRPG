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
} player_t;

/* **************** global functions ****************
 * see player.h for comments about exported functions
 */

/**************** player_new() ****************/
player_t *
player_new()
{
	player_t *player = calloc(1, sizeof(player_t));

	if (player == NULL) {
		return player;
	}

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
	player->row = 0;
	player->col = 0;
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