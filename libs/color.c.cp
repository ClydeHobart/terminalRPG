/* color.c
 * A set of functions for initializing initials[0] and declaring color constants for
 * YTU. See color.h for more information regarding individual functions
 *
 * Zeke Baker | 20180716.2300 | terminalRPG/libs/color.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

const int C_BLACK					= 0;
const int C_GRAY					= 4;
const int C_WHITE					= 8;
const int C_RED						= 12;
const int C_ORANGE				= 20;
const int C_YELLOW				= 28;
const int C_CHARTREUSE		= 36;
const int C_GREEN					= 44;
const int C_SPRING_GREEN	= 52;
const int C_CYAN					= 60;
const int C_AZURE					= 68;
const int C_BLUE					= 76;
const int C_VIOLET				= 84;
const int C_MAGENTA				= 92;
const int C_HOT_PINK			= 100;

/* **************** global functions ****************
 * See animations.h for function descriptions
 */
char **initColors(void);
void resetColors(int ***initials);

/**************** local functions ****************/
int adjustComp(int comp, int dc);

/**************** initColors() ****************/
char **
initColors(void)
{
	char **colors;
	int colorVals[13][3] =
		{	{	127,	127,	127	},
			{	255,	0,		0		},
			{	255,	127,	0		},
			{	255,	255,	0		},
			{	127,	255,	0		},
			{	0,		255,	0		},
			{	0,		255,	127	},
			{	0,		255,	255	},
			{	0,		127,	255	},
			{	0,		0,		255	},
			{	127,	0,		255	},
			{	255,	0,		255	},
			{	255,	0,		127	}};

	colors = calloc(104, sizeof(char *));

	for (int r = 0; r < 13; r++) {
		for (int dc = -3; dc < 4; dc++) {
			colors[8 * r + dc + 4] = calloc(20, sizeof(char));
			sprintf(colors[8 * r + dc + 4], "\x1b[38;2;%.3d;%.3d;%.3dm",
				adjustComp(colorVals[r][0], dc),
				adjustComp(colorVals[r][1], dc),
				adjustComp(colorVals[r][2], dc));
		}
	}

	colors[C_BLACK] = calloc(20, sizeof(char));
	colors[C_WHITE] = calloc(20, sizeof(char));
	sprintf(colors[C_BLACK], "\x1b[38;2;000;000;000m");
	sprintf(colors[C_WHITE], "\x1b[38;2;255;255;255");

	return colors;
}

/* **************** adjustColor() ****************
 * Adjusts a color to a lighter or darker color
 *
 * color: int of color value (r, g, or b in [0, 1000]) to adjust
 * dc: int of amount to change color by (-3 through 3)
 * return: int of adjusted color value in [0, 1000]
 */
int adjustComp(int comp, int dc)
{
	if ((comp == 255 && dc > 0) || (comp == 0 && dc < 0) || dc == 0) return comp;

	if (dc < 0 ) {
		return comp * (4 + dc) / 4;
	} else {
		return 255 - (256 - comp) * (4 - dc) / 4;
	}
}

// \x1b[38;2;   ;   ;   m