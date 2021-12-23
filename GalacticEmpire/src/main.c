#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <tgi.h>
#include "graphics.h"


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
unsigned MaxX;
unsigned MaxY;

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
 * Main galactic empire game logic.
 */
void game() {
    // Initialize everything that shouldn't be changed on the map.
    initGameGraphics(MaxX, MaxY);

    // TODO: add map, input and further game functionality here and replace with game while loop
    updateTable(MaxX, MaxY, year);
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