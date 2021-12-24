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

#define COLOR_BORD      COLOR_GREEN


/*****************************************************************************/
/*                            Custom datatypes                               */
/*****************************************************************************/


struct world {
    unsigned x;
    unsigned y;
    char owner[3];
    unsigned prod;
    unsigned ships;
};


/*****************************************************************************/
/*                            Global variables                               */
/*****************************************************************************/

// Screen dimensions. // TODO: delete
unsigned MaxX = 320;
unsigned MaxY = 200;

// Game specific variables.
unsigned numPlayers = 5;
char *playerNames[5];
unsigned numWorlds = 10;
unsigned year = 0;
unsigned totalYears = 0;
int defensiveShips = 1;
int events = 1;
struct world galaxy[40];

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
 * Initializes the world. // TODO: here obviously a smart algorithm is needed: spheres, tree, random?
 */
void generateGalaxy(){
    // Loop variables.
    int i, j;
    int player = rand() % numPlayers;

    // Iteratively generate the game galaxy.
    for(i = 0; i < numWorlds; i++){
        galaxy[i].x = rand() % 15;
        galaxy[i].y = rand() % 15;
        galaxy[i].prod = rand() % 11;
        galaxy[i].ships = ((rand() % 5) + 1) * galaxy[i].prod;

        // Set owner of planet
        for(j = 0; j < 3; j++){
            galaxy[i].owner[j] = playerNames[player][j];
        }
    }
}

/**
 * Initialises the game questions.
 */
void initGameInputs(){
    // Loop variables.
    int i;

    // Get number of players.
    numPlayers = getNumPlayers();

    // Get player names.
    for(i = 0; i < numPlayers; i++){
        playerNames[i] = getPlayerName(i);
    }

    // Get number of worlds.
    numWorlds = getNumWorlds();

    // Get number of totalYears.
    totalYears = getYears();

    // Get special events settings.
    defensiveShips = getDefensive();
    events = getEvents();
}


/**
 * Main galactic empire game logic.
 */
void game() {
    // Plot start screen.
    startScreen(MaxX, MaxY);
    tgi_clear();

    // Handle initial questions.
    initGameInputs();
    tgi_clear();

    // Initialize everything that shouldn't be changed on the map.
    initGameGraphics(MaxX, MaxY);

    // Initialize world and map based on player acceptance.
    generateGalaxy();
    while(mapAcceptance()){
        updateMap();//TODO: (indicies, galaxy);
    }

    // Play the game until running out of years.
    while(year != totalYears){
        retrieveInputs();
        updateTable(MaxX, MaxY, year);
        updateMap();
    }

    // TODO: display final stats like winner etc.
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

    // Set the border colors.
    Border = bordercolor(COLOR_BORD);

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