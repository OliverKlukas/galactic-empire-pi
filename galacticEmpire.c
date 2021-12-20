#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <stdbool.h>
#include <tgi.h>

/*****************************************************************************/
/*                                 Macros                                    */
/*****************************************************************************/


#ifndef DYN_DRV
#  define DYN_DRV       0
#endif

#define COLOR_BACK      TGI_COLOR_GREEN
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


// Display variables
static unsigned MaxX;
static unsigned MaxY;
static unsigned AspectRatio;

// Game specific variables
int numPlayers = 1;
char *playerNames;
int numWorlds = 10;
int year = 0;
bool defensiveShips = true;
int events = true;
struct World **worlds;


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
 * TODO: get from Basti's branch
 */
static void drawMap(void) {}

/**
 * TODO
 */
static void updateInput(void) {}

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed graphics based on the global variables. Read only.
 */
static void drawTable(void) {
    // Margin of outer table rectangle.
    int margin = 10;

    // Print the outer lines of the table.
    tgi_gotoxy((3 * MaxX / 4), margin);
    tgi_lineto((MaxX / 2) + margin, margin);
    tgi_lineto((MaxX / 2) + margin, MaxY - (2 * margin));
    tgi_lineto((3 * MaxX / 4), MaxY - (2 * margin));
    tgi_lineto((3 * MaxX / 4), margin);
    tgi_lineto(MaxX - margin, margin);
    tgi_lineto(MaxX - margin, MaxY - (2 * margin));
    tgi_lineto((3 * MaxX / 4), MaxY - (2 * margin));

    // Print the current game year.
    cputsxy(10, 0, "A");
    tgi_outtextxy((MaxX / 2) + margin, MaxY - margin, "Year : ");
    tgi_outtext("0");
}

/**
 * Main galactic empire game logic.
 */
static void game(void){
    // Draw the current table and year.
    drawTable();

    // TODO: add map, input and further game functionality here and replace with game while loop
    drawMap();
    updateInput();
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

    // Set the palette, set the border color.
    Border = bordercolor (COLOR_BORD);

    // Clear the initial tgi drawing board.
    tgi_clear ();

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
    (void) bordercolor (Border);

    printf("Done\n");
    return EXIT_SUCCESS;
}
