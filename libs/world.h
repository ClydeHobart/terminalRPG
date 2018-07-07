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
 * return: world_t pointer to newly instantiated world
 *
 * memory contract: world_t pointer must be freed
 */
world_t *world_new(player_t *player);

/* **************** world_print() ****************
 * Prints contents of the world into worldWin
 * 
 * assumptions:
 * * worldWin is 27 x 89
 * * map files are 64 x 128
 */
void world_print(WINDOW *worldWin, world_t *world);

#endif // __WORLD_H__