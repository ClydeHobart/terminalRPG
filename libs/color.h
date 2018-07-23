/* color.h
 * A set of functions for initializing colors and declaring color constants for
 * YTU.
 *
 * Zeke Baker | 20180716.2326 | terminalRPG/libs/color.h
 */

#ifndef __COLORS_H__
#define __COLORS_H__

#include <stdio.h>
#include <ncurses.h>

/**************** functions ****************/

/* **************** initColors() ****************
 * Initializes colors and pairs for use.
 *
 * Assumptions:
 * * initscr() and start_color() has been called prior to this function
 */
short ***initColors(void);

/* **************** resetColors() ****************
 * Resets colors to initial states
 */
void resetColors(short ***initials);

int adjustColor(int color, int dc);

#endif // __COLORS_H__