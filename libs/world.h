/* Yoreland: Time Untold | world.c
 *
 * The header file for the world module, containing the world struct definition
 * and its related function definitions
 *
 * Zeke Baker | 20180707.0023 | terminalRPG/libs/world.h
 */

#ifndef __WORLD_H__
#define __WORLD_H__

#include <stdio.h>
#include <ncurses.h>
#include "player.h"

/**************** types ****************/
typedef struct world world_t;

/**************** functions ****************/

/* **************** world_new() ****************
 * Instantiates a new world at 0, 0 in chunk 0, 0
 *
 * worldWin: WINDOW pointer for world to print to
 * player: player_t pointer to the players data
 * return: world_t pointer to newly instantiated world
 *
 * memory contract: world_t pointer must be freed
 */
world_t *world_new(WINDOW *worldWin, player_t *player);

/* **************** world_print() ****************
 * Prints contents of the world into worldWin
 * 
 * world: world_t pointer of world to print
 *
 * assumptions:
 * * worldWin is 27 x 89
 * * map files are 64 x 128
 */
void world_print(world_t *world);

/* **************** world_handleMove() ****************
 * Handles move request from the player, printing if it's a valid move
 *
 * world: world_t pointer of world to validate move in
 * move: int of directional key pressed
 */
void world_handleMove(world_t *world, int move);

/* **************** world_save() ****************
 * Saves a world and its contents
 *
 * world: world_t pointer of world to save
 * return: bool of whether world saved properly
 *
 * assumptions: "saves" directory exists in current directory
 */
bool world_save(world_t *world);

/* **************** world_load() ****************
 * Loads a world and its contents
 *
 * win: WINDOW pointer of window to print to
 * player: player_t pointer of player in world
 * return: world_t pointer of loaded world, NULL if error encountered
 */
world_t *world_load(WINDOW *win, player_t *player);

#endif // __WORLD_H__