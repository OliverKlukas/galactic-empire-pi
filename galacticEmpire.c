#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <tgi.h>
#include "font8x8_basic.h"


/*****************************************************************************/
/*                                 Macros                                    */
/*****************************************************************************/


#ifndef DYN_DRV
#  define DYN_DRV       0
#endif

#define COLOR_BACK      TGI_COLOR_BLACK
#define COLOR_FORE      TGI_COLOR_WHITE
#define COLOR_BORD      COLOR_GREEN


/*****************************************************************************/
/*                            Custom datatypes                               */
/*****************************************************************************/


struct World {
    int x;
    int y;
    char owner[4];
    int prod;
    int ships;
};


/*****************************************************************************/
/*                            Global variables                               */
/*****************************************************************************/


// Display variables.
static unsigned MaxX;
static unsigned MaxY;
static unsigned AspectRatio;

// Game specific variables.
int numPlayers = 1;
char *playerNames;
int numWorlds = 10;
int year = 0;
int defensiveShips = 1;
int events = 1;
struct World **worlds;

// Globally used color Palette.
static const unsigned char ColorPalette[2] = {COLOR_FORE, COLOR_BACK};

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
 * Custom function to print out tgi errors.
 *
 * <p>Source: https://github.com/cc65/cc65/blob/master/samples/tgidemo.c
 *
 * @param S - Function name that throws the error given as String.
 */
static void CheckError(const char *S) {
    unsigned char Error = tgi_geterror();

    if (Error != TGI_ERR_OK) {
        printf("%s: %u\n", S, Error);
        if (doesclrscrafterexit()) {
            cgetc();
        }
        exit(EXIT_FAILURE);
    }
}


#if DYN_DRV
/**
 * Warns the user that the dynamic TGI driver is needed for this program.
 *
 * <p>Source: https://github.com/cc65/cc65/blob/master/samples/tgidemo.c
 */
static void DoWarning ()
/*  */
{
    printf ("Warning: This program needs the TGI\n"
            "driver on disk! Press 'y' if you have\n"
            "it - any other key exits.\n");
    if (tolower (cgetc ()) != 'y') {
        exit (EXIT_SUCCESS);
    }
    printf ("OK. Please wait patiently...\n");
}
#endif


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
static void plotLetter(unsigned x, unsigned y, int letter) { // TODO COLOR

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
                tgi_setcolor(COLOR_FORE);
                tgi_setpixel(x + j, y + i);
            } else {
                tgi_setcolor(COLOR_BACK);
                tgi_setpixel(x + j, y + i);
            }
        }
    }

    // Restore current foreground color.
    tgi_setcolor(COLOR_FORE);
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
static void plotText(unsigned x, unsigned y, char *sentence){
    // Loop variables.
    int i;

    // Iterate over the sentence and plot each char.
    for(i = 0; sentence[i] != 0; i++){
        plotLetter(x + i * letterSpacing, y, sentence[i]);
    }
}

/**
 * TODO: print all characters new or rather just the colors? Probably need to reprint them right?
 */
static void updateMap() {
    plotLetter(margin - centerLetter, margin - centerLetter, 'a'); // TODO figure out placement here
    plotLetter(margin - centerLetter + 2 * mapSquareSize, margin - centerLetter + 2 * mapSquareSize, 'c');
    plotLetter(margin - centerLetter + 3 * mapSquareSize, margin - centerLetter + 2 * mapSquareSize, 'F');
    plotLetter(margin - centerLetter, margin - centerLetter, 'b');

}

/**
 * TODO: print and update the current user input
 */
static void updateInput() {

}

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed graphics based on the global variables. Read only.
 * // TODO: this is quite expensive as I draw by pixel, can I only update the ones that are changed?
 */
static void updateTable() {
    // Loop variables.
    int row, column;

    // Table text coordinates.
    char letter = 'a';
    int tableTextX = (MaxX / 2) + (2 * margin);
    int tableTextY = margin + 2;

    // Update current year.
    if (year < 10) { // TODO: make a custom plot numbers
        plotLetter(tableTextX + 5 * letterSpacing, MaxY - (2 * margin), year + numOffset);
    } else {
        plotLetter(tableTextX + 5 * letterSpacing, MaxY - (2 * margin), (year / 10) + numOffset);
        plotLetter(tableTextX + 6 * letterSpacing, MaxY - (2 * margin), (year % 10) + numOffset);
    }

    // Update first column of current planet occupations.
    for(column = 0; column < 2; column++){
        for (row = 1; row < 22; row++) {
            // World.
            plotLetter(tableTextX, tableTextY + row * letterSpacing, letter);
            letter += 1;

            // Production. // TODO: make a custom plot numbers
            plotLetter(tableTextX + 3 * letterSpacing, tableTextY + row * letterSpacing, 9 + numOffset);

            // Ships.
            plotLetter(tableTextX + 7 * letterSpacing, tableTextY + row * letterSpacing, 8 + numOffset);
        }
        tableTextX += 75;
        letter = 'A';
    }
}

/**
 * Initializes the standard game graphics.
 */
static void initGameGraphics() {
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
        plotLetter(tableTextX, tableTextY, 'W');
        plotText(tableTextX + 2 * letterSpacing, tableTextY, "Pn");
        plotText(tableTextX + 5 * letterSpacing, tableTextY, "Shp");
    }

    // Print the game year.
    plotText((MaxX / 2) + (2 * margin), MaxY - (2 * margin), "Year:");

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

/**
 * Main galactic empire game logic.
 */
static void game() {
    // Initialize everything that shouldn't be changed on the map.
    initGameGraphics();

    // TODO: add map, input and further game functionality here and replace with game while loop
    updateTable();
    updateMap();
    updateInput();

    // TODO: replace by game loop
    cgetc();
}



/*****************************************************************************/
/*                                Main                                       */
/*****************************************************************************/

/**
 * Checks for driver compatibility and starts the Game
 *
 * <p>Partially adapted from: https://github.com/cc65/cc65/blob/master/samples/tgidemo.c
 *
 * @return Returns SUCCESS in case of completed game or FAILURE in case or errors.
 */
int main() {
    // Save the previous border color to reset later on.
    unsigned char Border;

#if DYN_DRV
    // Warn the user that the tgi driver is needed.
    DoWarning ();
    // Load and initialize the driver.
    tgi_load_driver (tgi_stddrv);
    CheckError ("tgi_load_driver");
#else
    // Install the driver.
    tgi_install(tgi_static_stddrv);
    CheckError("tgi_install");
#endif

    // Initialize the loaded graphics driver.
    tgi_init();
    CheckError("tgi_init");

    // Set global resolution variables.
    MaxX = tgi_getmaxx();
    MaxY = tgi_getmaxy();
    AspectRatio = tgi_getaspectratio();

    // Set the palette and border colors.
    Border = bordercolor(COLOR_BORD);
    tgi_setpalette(ColorPalette);

    // Clear the initial tgi drawing board.
    tgi_clear();

    // Call main game function.
    game();

#if DYN_DRV
    // Unload the driver.
    tgi_unload ();
#else
    // Uninstall the driver.
    tgi_uninstall();
#endif

    // Reset the border.
    (void) bordercolor(Border);

    printf("Done\n");
    return EXIT_SUCCESS;
}
