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
void placeColoredLetter(unsigned char x, unsigned char y, char character, unsigned char player) {
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
void placeColoredNumber(unsigned char x, unsigned char y, unsigned number, unsigned char player) {
    // Copy variables.
    unsigned digit;
    unsigned num = number;
    unsigned char i = 0;

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
    unsigned char i, j;

    // Set colors for the game.
    bgcolor(backgroundColor);
    textcolor(textColor);
    clrscr();

    // Draw the map grid.
    for (i = 0; i < MAP_N_LINES_HORIZONTAL; ++i) { // y
        for (j = 0; j < MAP_N_LINES_VERTICAL; ++j) { // x
            if (j == 0 && i == 0) {
                cputcxy(j, i, CH_ULCORNER);
            } else if (j == MAP_N_LINES_VERTICAL - 1 && i == 0) {
                cputcxy(j, i, CH_URCORNER);
            } else if (i == 0) {
                cputcxy(j, i, CH_TTEE);
            } else if (i == MAP_N_LINES_HORIZONTAL - 1 && j == 0) {
                cputcxy(j, i, CH_LLCORNER);
            } else if (i == MAP_N_LINES_HORIZONTAL - 1 && j == MAP_N_LINES_VERTICAL - 1) {
                cputcxy(j, i, CH_LRCORNER);
            } else if (j == 0) {
                cputcxy(j, i, CH_LTEE);
            } else if (i == MAP_N_LINES_HORIZONTAL - 1) {
                cputcxy(j, i, CH_BTEE);
            } else if (j == MAP_N_LINES_VERTICAL - 1) {
                cputcxy(j, i, CH_RTEE);
            } else {
                cputcxy(j, i, CH_CROSS);
            }

        }
    }

    // Draw the table border.
    cputcxy(MAP_N_LINES_VERTICAL, 0, CH_ULCORNER);
    chlinexy(MAP_N_LINES_VERTICAL + 1, 0, MAX_X - MAP_N_LINES_VERTICAL - 1);
    cputcxy(MAX_X, 0, CH_URCORNER);
    cvlinexy(MAX_X, 1, 22);
    cputcxy(MAX_X, 23, CH_LRCORNER);
    chlinexy(MAP_N_LINES_VERTICAL + 1, 23, MAX_X - MAP_N_LINES_VERTICAL - 1);
    cputcxy(MAP_N_LINES_VERTICAL, 23, CH_LLCORNER);
    cvlinexy(MAP_N_LINES_VERTICAL, 1, 22);

    // Draw the table middle lines.
    cvlinexy(MAP_N_LINES_VERTICAL + (MAX_X - MAP_N_LINES_VERTICAL) / 2, 1, 22);
    cvlinexy(MAP_N_LINES_VERTICAL + (MAX_X - MAP_N_LINES_VERTICAL) / 2 + 1, 1, 22);
    cputcxy(MAP_N_LINES_VERTICAL + (MAX_X - MAP_N_LINES_VERTICAL) / 2, 0, CH_TTEE);
    cputcxy(MAP_N_LINES_VERTICAL + (MAX_X - MAP_N_LINES_VERTICAL) / 2 + 1, 0, CH_TTEE);
    cputcxy(MAP_N_LINES_VERTICAL + (MAX_X - MAP_N_LINES_VERTICAL) / 2, 23, CH_BTEE);
    cputcxy(MAP_N_LINES_VERTICAL + (MAX_X - MAP_N_LINES_VERTICAL) / 2 + 1, 23, CH_BTEE);

    // Draw the table header.
    cputsxy(MAP_N_LINES_VERTICAL + 1, 1, "W Pr Shp");
    cputsxy(MAP_N_LINES_VERTICAL + (MAX_X - MAP_N_LINES_VERTICAL) / 2 + 2, 1, "W Pr Shp");
    chlinexy(MAP_N_LINES_VERTICAL + 1, 2, MAX_X - MAP_N_LINES_VERTICAL - 1);
    cputcxy(MAP_N_LINES_VERTICAL, 2, CH_LTEE);
    cputcxy(MAX_X, 2, CH_RTEE);
    cputcxy(MAP_N_LINES_VERTICAL + (MAX_X - MAP_N_LINES_VERTICAL) / 2, 2, CH_CROSS);
    cputcxy(MAP_N_LINES_VERTICAL + (MAX_X - MAP_N_LINES_VERTICAL) / 2 + 1, 2, CH_CROSS);

    // Draw the year.
    cputsxy(YEAR_LINE_X, YEAR_LINE_Y, "Year:");

    // Draw input text box.
    cputcxy(0, MAP_N_LINES_HORIZONTAL, CH_ULCORNER);
    chlinexy(1, MAP_N_LINES_HORIZONTAL, MAP_N_LINES_VERTICAL - 2);
    cputcxy(MAP_N_LINES_VERTICAL - 1, MAP_N_LINES_HORIZONTAL, CH_URCORNER);
    cvlinexy(MAP_N_LINES_VERTICAL - 1, MAP_N_LINES_HORIZONTAL + 1, MAX_Y - MAP_N_LINES_HORIZONTAL - 1);
    cputcxy(MAP_N_LINES_VERTICAL - 1, MAX_Y, CH_LRCORNER);
    chlinexy(1, MAX_Y, MAP_N_LINES_VERTICAL - 2);
    cputcxy(0, MAX_Y, CH_LLCORNER);
    cvlinexy(0, MAP_N_LINES_HORIZONTAL + 1, MAX_Y - MAP_N_LINES_HORIZONTAL - 1);
}
