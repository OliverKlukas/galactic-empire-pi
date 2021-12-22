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

// Space between alphabet letters.
int charSpacing = 8;

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
static void DoWarning (void)
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
 * @param x - X coordinate of letter center. // TODO: is this really the center?
 * @param y - Y coordinate of letter center
 * @param letter - Letter to be plotted.
 */
static void plotLetter(unsigned char x, unsigned char y, int letter) { // TODO COLOR

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
 * TODO: print all characters new or rather just the colors? Probably need to reprint them right?
 */
static void updateMap(void) {

}

/**
 * TODO: print and update the current user input
 */
static void updateInput(void) {

}

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed graphics based on the global variables. Read only.
 */
static void updateTable(void) {
    // Update current year
    if (year < 10) {
        plotLetter((MaxX / 2) + (2 * margin) + 5 * charSpacing, MaxY - (2 * margin), year + 48);
    } else {
        plotLetter((MaxX / 2) + (2 * margin) + 5 * charSpacing, MaxY - (2 * margin), (year / 10) + 48);
        plotLetter((MaxX / 2) + (2 * margin) + 6 * charSpacing, MaxY - (2 * margin), (year % 10) + 48);
    }

    // TODO: Update current planet occupations.
}

/**
 * Initializes the standard game graphics.
 */
static void initGame(void) {
    // Init parameters.
    int i, j, boxLeftX, boxRightX, boxUpperY, boxLowerY;

    // Print the outer lines of the table.
    tgi_gotoxy((3 * MaxX / 4), margin);
    tgi_lineto((MaxX / 2) + margin, margin);
    tgi_lineto((MaxX / 2) + margin, MaxY - (3 * margin));
    tgi_lineto((3 * MaxX / 4), MaxY - (3 * margin));
    tgi_lineto((3 * MaxX / 4), margin);
    tgi_lineto(MaxX - margin, margin);
    tgi_lineto(MaxX - margin, MaxY - (3 * margin));
    tgi_lineto((3 * MaxX / 4), MaxY - (3 * margin));

    // Print the game year.
    plotLetter((MaxX / 2) + (2 * margin), MaxY - (2 * margin), 'Y');
    plotLetter((MaxX / 2) + (2 * margin) + 1 * charSpacing, MaxY - (2 * margin), 'e');
    plotLetter((MaxX / 2) + (2 * margin) + 2 * charSpacing, MaxY - (2 * margin), 'a');
    plotLetter((MaxX / 2) + (2 * margin) + 3 * charSpacing, MaxY - (2 * margin), 'r');
    plotLetter((MaxX / 2) + (2 * margin) + 4 * charSpacing, MaxY - (2 * margin), ':');

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
    boxUpperY = mapNLinesHorizontal * (mapLineThickness + mapSquareSize) + mapLineThickness + (2*margin);
    boxLowerY = MaxY - margin;
    boxLeftX = margin;
    boxRightX = (MaxX / 2) - margin + 1;

    tgi_line(boxLeftX, boxUpperY, boxRightX, boxUpperY);
    tgi_line(boxLeftX, boxUpperY, boxLeftX, boxLowerY);
    tgi_line(boxLeftX, boxLowerY, boxRightX, boxLowerY);
    tgi_line(boxRightX, boxUpperY, boxRightX, boxLowerY);
}

/**
 * Main galactic empire game logic.
 */
static void game(void) {
    // Initialize everything that shouldn't be changed on the map.
    initGame();

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
