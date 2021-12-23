#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <tgi.h>
#include "font8x8_basic.h"



/*****************************************************************************/
/*                                 Macros                                    */
/*****************************************************************************/

#define COLOR_BACK      TGI_COLOR_BLACK
#define COLOR_FORE      TGI_COLOR_WHITE


/*****************************************************************************/
/*                            Global variables                               */
/*****************************************************************************/


// Map settings.
int mapNLinesVertical = 15;
int mapNLinesHorizontal = 15;
int mapLineThickness = 1;
int mapSquareSize = 9;

// Outer margin of game.
int margin = 5;

// Number offset for plotting digits.
int numOffset = 48;

// Space between alphabet letters.
int letterSpacing = 8;

// Center 8x8 letters to coordinates.
int centerLetter = 4;

/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/

/**
 * Places a character anywhere on the screen.
 *
 * <p>Write given letter bit by bit onto the screen while overwriting the content that was located at the given x and y.
 * The letters are 8x8 dimensioned and most ascii signs are supported.
 *
 * @param x - X coordinate of letter, upper left corner.
 * @param y - Y coordinate of letter, upper left corner.
 * @param letter - Letter to be plotted.
 */
void plotLetter(unsigned x, unsigned y, int letter, unsigned char colorForeground, unsigned char colorBackground) {

    // Variables to plot pixels.
    int i, j, plot;
    char *bitmap;

    // Receive bit config of letter with moder ascii.
    if (letter > 192 && letter < 219) {
        bitmap = font8x8_basic[letter - 128];
    } else if (letter > 64 && letter < 91) {
        bitmap = font8x8_basic[letter + 32];
    } else {
        bitmap = font8x8_basic[letter];
    }

    // Render letter bit by bit onto map.
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            plot = bitmap[i] & 1 << j;
            if (plot) {
                tgi_setcolor(colorForeground);
                tgi_setpixel(x + j, y + i);
            } else {
                tgi_setcolor(colorBackground);
                tgi_setpixel(x + j, y + i);
            }
        }
    }

    // Restore current foreground color.
    tgi_setcolor(colorForeground);
}

/**
 * Writes a sentence on the screen.
 *
 * <p>Write given sentence bit by bit onto the screen while overwriting the content that was located at the given x and
 * y. The letters are 8x8 dimensioned and most ascii signs are supported. Distance between letters depends on spacing.
 *
 * @param x - Start X coordinate of sentence.
 * @param y - Start Y coordinate of sentence.
 * @param sentence - String Sentence to be plotted.
 */
void plotText(unsigned x, unsigned y, char *sentence, unsigned char colorForeground, unsigned char colorBackground){
    // Loop variables.
    int i;

    // Iterate over the sentence and plot each char.
    for(i = 0; sentence[i] != 0; i++){
        plotLetter(x + i * letterSpacing, y, sentence[i], colorForeground, colorBackground);
    }
}

/**
 * TODO: print all characters new or rather just the colors? Probably need to reprint them right?
 */
void updateMap() {
    plotLetter(margin - centerLetter, margin - centerLetter, 'a', COLOR_FORE, COLOR_BACK); // TODO figure out placement here
    plotLetter(margin - centerLetter + 2 * mapSquareSize, margin - centerLetter + 2 * mapSquareSize, 'c', COLOR_FORE, COLOR_BACK);
    plotLetter(margin - centerLetter + 3 * mapSquareSize, margin - centerLetter + 2 * mapSquareSize, 'F', COLOR_FORE, COLOR_BACK);
    plotLetter(margin - centerLetter, margin - centerLetter, 'b', COLOR_FORE, COLOR_BACK);

}

/**
 * TODO: print and update the current user input
 */
void updateInput() {

}

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed graphics based on the global variables. Read only.
 * // TODO: this is quite expensive as I draw by pixel, can I only update the ones that are changed?
 */
void updateTable(unsigned MaxX, unsigned MaxY, unsigned year) {
    // Loop variables.
    int row, column;

    // Table text coordinates.
    char letter = 'a';
    int tableTextX = (MaxX / 2) + (2 * margin);
    int tableTextY = margin + 2;

    // Update current year.
    if (year < 10) { // TODO: make a custom plot numbers
        plotLetter(tableTextX + 5 * letterSpacing, MaxY - (2 * margin), year + numOffset, COLOR_FORE, COLOR_BACK);
    } else {
        plotLetter(tableTextX + 5 * letterSpacing, MaxY - (2 * margin), (year / 10) + numOffset, COLOR_FORE, COLOR_BACK);
        plotLetter(tableTextX + 6 * letterSpacing, MaxY - (2 * margin), (year % 10) + numOffset, COLOR_FORE, COLOR_BACK);
    }

    // Update first column of current planet occupations.
    for(column = 0; column < 2; column++){
        for (row = 1; row < 22; row++) {
            // World.
            plotLetter(tableTextX, tableTextY + row * letterSpacing, letter, COLOR_FORE, COLOR_BACK);
            letter += 1;

            // Production. // TODO: make a custom plot numbers
            plotLetter(tableTextX + 3 * letterSpacing, tableTextY + row * letterSpacing, 9 + numOffset, COLOR_FORE, COLOR_BACK);

            // Ships.
            plotLetter(tableTextX + 7 * letterSpacing, tableTextY + row * letterSpacing, 8 + numOffset, COLOR_FORE, COLOR_BACK);
        }
        tableTextX += 75;
        letter = 'A';
    }
}

/**
 * Initializes the standard game graphics.
 */
void initGameGraphics(unsigned MaxX, unsigned MaxY) {
    // Loop parameters.
    int i, j, column;

    // Table text (X, Y) params.
    int tableTextX = (MaxX / 2) + (2 * margin);
    int tableTextY = margin + 2;

    // Table corner (X, Y) params.
    int tableUpperY = margin;
    int tableLowerY = MaxY - (3 * margin);
    int tableLeftX = (MaxX / 2) + margin;
    int tableRightX = MaxX - margin;

    // Input box corner (X, Y) params.
    int boxUpperY = mapNLinesHorizontal * (mapLineThickness + mapSquareSize) + mapLineThickness + (2 * margin);
    int boxLowerY = MaxY - margin;
    int boxLeftX = margin;
    int boxRightX = (MaxX / 2) - margin + 1;

    // Print the outer lines of the table.
    tgi_line(tableLeftX, tableUpperY, tableRightX, tableUpperY);
    tgi_line(tableLeftX, tableUpperY, tableLeftX, tableLowerY);
    tgi_line(tableLeftX, tableLowerY, tableRightX, tableLowerY);
    tgi_line(tableRightX, tableUpperY, tableRightX, tableLowerY);
    tgi_line((3 * MaxX / 4), tableUpperY, (3 * MaxX / 4), tableLowerY);

    // Print first row of the table.
    for(column = 0; column < 2; column++){
        tableTextX += column * 75;
        plotLetter(tableTextX, tableTextY, 'W', COLOR_FORE, COLOR_BACK);
        plotText(tableTextX + 2 * letterSpacing, tableTextY, "Pn", COLOR_FORE, COLOR_BACK);
        plotText(tableTextX + 5 * letterSpacing, tableTextY, "Shp", COLOR_FORE, COLOR_BACK);
    }

    // Print the game year.
    plotText((MaxX / 2) + (2 * margin), MaxY - (2 * margin), "Year:", COLOR_FORE, COLOR_BACK);

    // Print map vertical lines.
    for (i = 0; i <= mapNLinesVertical; ++i) {
        for (j = 0; j < mapLineThickness; ++j) {
            tgi_line(i * (mapLineThickness + mapSquareSize) + j + margin, margin,
                     i * (mapLineThickness + mapSquareSize) + j + margin,
                     mapNLinesHorizontal * (mapLineThickness + mapSquareSize) + margin);
        }
    }

    // Print map horizontal lines.
    for (i = 0; i <= mapNLinesHorizontal; ++i) {
        for (j = 0; j < mapLineThickness; ++j) {
            tgi_line(margin, i * (mapLineThickness + mapSquareSize) + j + margin,
                     mapNLinesVertical * (mapLineThickness + mapSquareSize) + margin,
                     i * (mapLineThickness + mapSquareSize) + j + margin);
        }
    }

    // Print textual output box.
    tgi_line(boxLeftX, boxUpperY, boxRightX, boxUpperY);
    tgi_line(boxLeftX, boxUpperY, boxLeftX, boxLowerY);
    tgi_line(boxLeftX, boxLowerY, boxRightX, boxLowerY);
    tgi_line(boxRightX, boxUpperY, boxRightX, boxLowerY);
}
