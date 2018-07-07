/* Yoreland: Time Untold | player.h
 * 
 * The header file for the player module, containing the player struct definition and its
 * related function definitions
 *
 * Zeke Baker | 20180705.1024 | terminalRPG/libs/player.h
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdio.h>

/**************** types ****************/
typedef struct player player_t;

/**************** functions ****************/

/* **************** player_new() ****************
 * Instantiates a new player with 1 for all stats, lvl 1, 0 exp, and 100 for all
 * other attributes
 *
 * return: player_t pointer to newly instantiated player
 * 
 * memory contract: player_t pointer must be freed
 */
player_t *player_new();

/* **************** player_getLoc() ****************
 * Returns the location of the player
 *
 * player: player_t pointer to the player whose location is requested
 * return: int pointer loc, where loc[0] is the row, and loc[1] is the column
 *
 * memory contract: int pointer must be freed
 */
int *player_getLoc(player_t *player);

#endif // __PLAYER_H__