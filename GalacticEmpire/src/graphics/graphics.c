#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
#include "graphics.h"
#include "../main.h"


/*****************************************************************************/
/*                        Global graphics variables                          */
/*****************************************************************************/


// Screen dimensions: these are the maximum pixel positions displayable
const unsigned maxX = 39;
const unsigned maxY = 24;

// Map settings.
const int mapNLinesVertical = 20;
const int mapNLinesHorizontal = 20;

// Year settings.
int yearLineX = 0;
int yearLineY = 0;

// Text field coordinates.
int textLine1X = 0;
int textLine1Y = 0;
int textLine2X = 0;
int textLine2Y = 0;

// Table coordinates.
int tableColumn1XMin = 0;
int tableColumn2XMin = 0;
int tableFirstRowYMin = 0;

// Globally used color Palettes.
const unsigned startTextColor = COLOR_WHITE;
const unsigned startBackgroundColor = COLOR_GREEN;
const unsigned textColor = COLOR_BLACK;
const unsigned backgroundColor = COLOR_WHITE;
const unsigned playerColors[6] = {COLOR_BLACK, COLOR_BLUE, COLOR_PURPLE, COLOR_GREEN, COLOR_RED, COLOR_BROWN};


/*****************************************************************************/
/*                        Global graphics functions                          */
/*****************************************************************************/


/**
 * Places colored letter on screen.
 *
 * @param x - X coordinate on screen, 0-39.
 * @param y - Y coordinate on screen, 0-24.
 * @param character - Character that should be plotted.
 * @param player - Player that owns the character plotted, 0 = pirate, 1-5 = players.
 */
void placeColoredLetter(unsigned x, unsigned y, char character, int player) {
    textcolor(playerColors[player]);
    cputcxy(x, y, character);
    textcolor(textColor);
}

/**
 *  Places colored number on screen.
 *
 *  <p>Places the number with its right most x value. Numbers grow with its decimal places to the left.
 *
 * @param x - Right most X coordinate that the number can take, 0-39
 * @param y - Y coordinate on screen, 0-24.
 * @param number - Number that should be plotted.
 * @param player - Player that owns the character plotted, 0 = pirate, 1-5 = players.
 */
void placeColoredNumber(unsigned x, unsigned y, unsigned number, unsigned player) {
    // Copy variables.
    int digit;
    int num = number;
    int i = 0;

    // Set text color according to selected player.
    textcolor(playerColors[player]);

    // Plot number by breaking it down into its single digits.
    if (number == 0) {
        cputcxy(x, y, '0');
    } else {
        while (num > 0) {
            digit = num % 10;
            cputcxy(x - i, y, digit + '0');
            num = num / 10;
            i++;
        }
    }

    // Reset color to last used one.
    textcolor(textColor);
}

/**
 * Initializes the standard game graphics.
 */
void initGameGraphics() {
    int i, j;

    int tableXMin = mapNLinesVertical;
    int tableXMax = maxX;
    int tableYMin = 0;
    int tableYMax = tableYMin + 20 + 3;

    int textFieldXMin = 0;
    int textFieldXMax = mapNLinesVertical - 1;
    int textFieldYMin = mapNLinesHorizontal;
    int textFieldYMax = maxY;

    // Set colors for the game.
    bgcolor(backgroundColor);
    textcolor(textColor);

    clrscr();
    //// 1. Draw map grid
    // y
    for (i = 0; i < mapNLinesHorizontal; ++i) {
        // x
        for (j = 0; j < mapNLinesVertical; ++j) {
            if (j == 0 && i == 0) {
                cputcxy(j, i, CH_ULCORNER);
            } else if (j == mapNLinesVertical - 1 && i == 0) {
                cputcxy(j, i, CH_URCORNER);
            } else if (i == 0) {
                cputcxy(j, i, CH_TTEE);
            } else if (i == mapNLinesHorizontal - 1 && j == 0) {
                cputcxy(j, i, CH_LLCORNER);
            } else if (i == mapNLinesHorizontal - 1 && j == mapNLinesVertical - 1) {
                cputcxy(j, i, CH_LRCORNER);
            } else if (j == 0) {
                cputcxy(j, i, CH_LTEE);
            } else if (i == mapNLinesHorizontal - 1) {
                cputcxy(j, i, CH_BTEE);
            } else if (j == mapNLinesVertical - 1) {
                cputcxy(j, i, CH_RTEE);
            } else {
                cputcxy(j, i, CH_CROSS);
            }

        }
    }

    /// draw table
    // border
    cputcxy(tableXMin, tableYMin, CH_ULCORNER);
    chlinexy(tableXMin + 1, tableYMin, tableXMax - tableXMin - 1);
    cputcxy(tableXMax, tableYMin, CH_URCORNER);
    cvlinexy(tableXMax, tableYMin + 1, tableYMax - tableYMin - 1);
    cputcxy(tableXMax, tableYMax, CH_LRCORNER);
    chlinexy(tableXMin + 1, tableYMax, tableXMax - tableXMin - 1);
    cputcxy(tableXMin, tableYMax, CH_LLCORNER);
    cvlinexy(tableXMin, tableYMin + 1, tableYMax - tableYMin - 1);

    tableColumn1XMin = tableXMin + 1;

    // middle lines
    cvlinexy(tableXMin + (tableXMax - tableXMin) / 2, tableYMin + 1, tableYMax - tableYMin - 1);
    cvlinexy(tableXMin + (tableXMax - tableXMin) / 2 + 1, tableYMin + 1, tableYMax - tableYMin - 1);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2, tableYMin, CH_TTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2 + 1, tableYMin, CH_TTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2, tableYMax, CH_BTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2 + 1, tableYMax, CH_BTEE);

    tableColumn2XMin = tableXMin + (tableXMax - tableXMin) / 2 + 2;

    // header
    cputsxy(tableXMin + 1, tableYMin + 1, "W Pr Shp");
    cputsxy(tableXMin + (tableXMax - tableXMin) / 2 + 2, tableYMin + 1, "W Pr Shp");
    chlinexy(tableXMin + 1, tableYMin + 2, tableXMax - tableXMin - 1);
    cputcxy(tableXMin, tableYMin + 2, CH_LTEE);
    cputcxy(tableXMax, tableYMin + 2, CH_RTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2, tableYMin + 2, CH_CROSS);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2 + 1, tableYMin + 2, CH_CROSS);
    tableFirstRowYMin = tableYMin + 3;

    /// Year 
    yearLineX = tableXMin + 1;
    yearLineY = tableYMax + 1;
    cputsxy(yearLineX, yearLineY, "Year:");

    /// Input text field
    cputcxy(textFieldXMin, textFieldYMin, CH_ULCORNER);
    chlinexy(textFieldXMin + 1, textFieldYMin, textFieldXMax - textFieldXMin - 1);
    cputcxy(textFieldXMax, textFieldYMin, CH_URCORNER);
    cvlinexy(textFieldXMax, textFieldYMin + 1, textFieldYMax - textFieldYMin - 1);
    cputcxy(textFieldXMax, textFieldYMax, CH_LRCORNER);
    chlinexy(textFieldXMin + 1, textFieldYMax, textFieldXMax - textFieldXMin - 1);
    cputcxy(textFieldXMin, textFieldYMax, CH_LLCORNER);
    cvlinexy(textFieldXMin, textFieldYMin + 1, textFieldYMax - textFieldYMin - 1);
    textLine1X = textFieldXMin + 1;
    textLine1Y = textFieldYMin + 1;
    textLine2X = textLine1X;
    textLine2Y = textLine1Y + 1;
}
