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
 * name: string of player name
 * return: player_t pointer to newly instantiated player
 * 
 * memory contract: player_t pointer must be freed
 */
player_t *player_new(const char *name);

/* **************** player_getLoc() ****************
 * Returns the location of the player
 *
 * player: player_t pointer to the player whose location is requested
 * return: int pointer loc, where loc[0] is the row, and loc[1] is the column
 *
 * memory contract: int pointer must be freed
 */
int *player_getLoc(player_t *player);

/* **************** player_getStats() ****************
 * Returns the stats of the player
 *
 * player: player_t pointer to the player whose stats are requested
 * return: int pointer stats, where stats are indexed according to the STAT
 *		constants
 *
 * memory contract: int pointer must be freed
 */
int *player_getStats(player_t *player);

/* **************** playeer_getMIG() ****************
 * Returns the might of the player
 *
 * player: player_t pointer to the player whose might is requested
 * return: int of player's might stat
 */
int player_getMIG(player_t *player);

/* **************** playeer_getVIG() ****************
 * Returns the vigor of the player
 *
 * player: player_t pointer to the player whose vigor is requested
 * return: int of player's vigor stat
 */
int player_getVIG(player_t *player);

/* **************** playeer_getLEG() ****************
 * Returns the legerity of the player
 *
 * player: player_t pointer to the player whose legerity is requested
 * return: int of player's legerity stat
 */
int player_getLEG(player_t *player);

/* **************** playeer_getINT() ****************
 * Returns the intellect of the player
 *
 * player: player_t pointer to the player whose intellect is requested
 * return: int of player's intellect stat
 */
int player_getINT(player_t *player);

/* **************** playeer_getJUD() ****************
 * Returns the judgement of the player
 *
 * player: player_t pointer to the player whose judgement is requested
 * return: int of player's judgement stat
 */
int player_getJUD(player_t *player);

/* **************** playeer_getCHA() ****************
 * Returns the charm of the player
 *
 * player: player_t pointer to the player whose charm is requested
 * return: int of player's charm stat
 */
int player_getCHA(player_t *player);

/* **************** playeer_getFAV() ****************
 * Returns the favor of the player
 *
 * player: player_t pointer to the player whose favor is requested
 * return: int of player's favor stat
 */
int player_getFAV(player_t *player);


/* **************** player_getName() ****************
 * Returns the name of the player
 *
 * player: player_t pointer to the player whose name is requested
 * return: alloc'ed string of the players name
 *
 * memory contract: returned string must be freed
 */
char *player_getName(player_t *player);

/* **************** player_move() ****************
 * Moves the player one unit in the specified direction
 *
 * player: player_t pointer of player to move
 * move: int of directional key pressed
 */
void player_move(player_t *player, int move);

/* **************** player_save() ****************
 * Saves a player's information
 *
 * player: player_t pointer of player to save
 *
 * assumptions: "saves" directory exists in current directory
 */
void player_save(player_t *player);

/* **************** player_load() ****************
 * Loads a player's information
 *
 * player: player_t pointer of player to save
 *
 * assumptions: "saves" directory exists in current directory
 */
player_t *player_load(const char *name);

#endif // __PLAYER_H__