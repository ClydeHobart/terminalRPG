/* animations.h
 * A set of functions to perform animations for YTU
 *
 * Zeke Baker | 20180702.0841 | terminalRPG/libs/animations.h
 */

#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__

#include <stdio.h>
#include <ncurses.h>

/**************** functions ****************/

/* **************** openingTitle_anim() ****************
 * Animates the opening title screen
 *
 * win: WINDOW * to print the animation to (probably stdscr)
 * return:
 * * 1: yoreland.txt didn't load properly
 *
 * assumptions: win has dimensions 40 x 120
 */
int openingTitle_anim(WINDOW* win);

#endif // __ANIMATIONS_H__