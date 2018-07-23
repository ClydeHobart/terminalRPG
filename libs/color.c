/* color.c
 * A set of functions for initializing initials[0] and declaring color constants for
 * YTU. See color.h for more information regarding individual functions
 *
 * Zeke Baker | 20180716.2300 | terminalRPG/libs/color.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "constants.h"

/* **************** global functions ****************
 * See animations.h for function descriptions
 */
short ***initColors(void);
void resetColors(short ***initials);
int adjustColor(int color, int dc);

/**************** local functions ****************/
static int adjustComp(int comp, int dc);

/**************** initColors() ****************/
short ***
initColors(void)
{
	short ***initials;

	initials = calloc(2, sizeof(short **));
	initials[0] = calloc(256, sizeof(short *));
	initials[1] = calloc(256, sizeof(short *));

	for (short c = 0; c < 256; c++) {
		initials[0][c] = calloc(3, sizeof(short));
		initials[1][c] = calloc(2, sizeof(short));
		color_content(c, &(initials[0][c][0]), &(initials[0][c][1]), &(initials[0][c][2]));
		pair_content(c, &(initials[1][c][0]), &(initials[1][c][1]));
		init_color(c, initials[0][c][0], initials[0][c][1], initials[0][c][2]);
	}

	for (short c = 1; c < 256; c++) {
		init_pair(c, c, 0);
	}

	return initials;
}

void
resetColors(short ***initials)
{
	for (short c = 0; c < 256; c++) {
		init_color(c, initials[0][c][0], initials[0][c][1], initials[0][c][2]);
		init_pair(c, initials[1][c][0], initials[1][c][1]);

		free(initials[0][c]);
		free(initials[1][c]);
	}
	free(initials[0]);
	free(initials[1]);
}

/* **************** adjustColor() ****************
 * Adjusts a color to a lighter or darker color
 *
 * color: short of color value (r, g, or b in [0, 1000]) to adjust
 * dc: short of amount to change color by (-3 through 3)
 * return: short of adjusted color value in [0, 1000]
 */
static int adjustComp(int comp, int dc)
{
	if ((comp == 5 && dc > 0) || (comp == 0 && dc < 0) || dc == 0) return comp;
	else if (dc < 0 ) {
		return comp * (5 + dc) / 5;
	} else {
		return 5 - (5 - comp) * (5 - dc) / 5;
	}
}

int adjustColor(int color, int dc) {
	int r;
	int g;
	int b;

	switch (color) {
		case YTU_COLOR_BLACK :
			if (dc > 0) {
				return 231 + 5 * dc;
			} else return color;
		case YTU_COLOR_WHITE :
			if (dc < 0) {
				return 256 + 5 * dc;
			} else return color;
		case YTU_COLOR_GRAY :
			return 244 + (int)(2.5 * dc);
		default :
			r = (color - 16) / 36;
			g = ((color - 16) / 6) % 6;
			b = (color - 16) % 6;
			return adjustComp(r, dc) * 36 + adjustComp(g, dc) * 6 + adjustComp(b, dc) + 16;
	}
}