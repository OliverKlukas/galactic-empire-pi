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
    unsigned x;             // 0-19 x axis position on map
    unsigned y;             // 0-19 y axis position on map
    unsigned owner;         // 0 is pirate, 1 - 5 players
    unsigned prod;          // 0 - x production
    unsigned ships;         // 0 - x number of ships
};


/*****************************************************************************/
/*                            Global variables                               */
/*****************************************************************************/


// Game specific variables.
unsigned numPlayers = 5;
char *playerNames[5];
unsigned numWorlds = 10;
unsigned year = 0;
unsigned totalYears = 0;
int defensiveShips = 1;
int events = 1;
struct world galaxy[40];

// Available planet locations.
int availableLocations[400];

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
 * Checks if a planet location is not already taken and occupies it if possible.
 *
 * @return - Returns 0 if available and 1 if not.
 */
unsigned occupyLocation(unsigned x, unsigned y) {
    if (availableLocations[x + y * 20] != 0) {
        // Location is not available.
        return 1;
    } else {
        // Location is available, occupy it.
        availableLocations[x + y * 20] = 1;
        return 0;
    }
}

/**
 * Generates a playable initial galaxy.
 */
void generateGalaxy() {
    // Loop variables.
    int i, j;
    unsigned planetIndex;

    // Scenario variables.
    unsigned maxProduction = 20;
    unsigned homePlanetProduction = 10;
    unsigned homePlanetBorderDistance = 2;
    unsigned homePlanetInitialShips = 200;
    unsigned totalProductionCapacity = maxProduction * (numWorlds / 2);
    unsigned excessProductionBonus = 5;

    // Available sphere divisions.
    unsigned sphereDivisions[][][] = {
            {
                    {0, 0, 19, 19},             // 1 player
                    {0,  0,  0,  0},
                    {0,  0,  0,  0},
                    {0,  0,  0,  0},
                    {0,  0, 0,  0}
            },
            {
                    {0, 0, 9,  9},              // 2 player
                    {10, 10, 19, 19},
                    {0,  0,  0,  0},
                    {0,  0,  0,  0},
                    {0,  0, 0,  0}
            },
            {
                    {0, 0, 19, 8},              // 3 player
                    {0,  9,  9,  19},
                    {10, 9,  19, 19},
                    {0,  0,  0,  0},
                    {0,  0, 0,  0}
            },
            {
                    {0, 0, 9,  9},              // 4 player
                    {10, 0,  19, 9},
                    {0,  10, 9,  19},
                    {10, 10, 19, 19},
                    {0,  0, 0,  0}
            },
            {
                    {0, 0, 3,  19},             // 5 player
                    {4,  0,  7,  19},
                    {8,  0,  11, 19},
                    {12, 0,  15, 19},
                    {16, 0, 19, 19}
            }

    };

    // Divide galaxy into spheres depending on player number.
    unsigned numWorldsPerSphere[5];
    int availableProduction[5];
    for (i = 0; i < numPlayers; i++) {
        numWorldsPerSphere[i] = numWorlds / numPlayers;
        availableProduction[i] = totalProductionCapacity / numPlayers;
    }

    // Account for not fitting world numbers.
    if (((numWorlds / numPlayers) * numPlayers) < numWorlds) {
        numWorldsPerSphere[numPlayers - 1] += 1;
    }

    // Build all player spheres.
    for (i = 0; i < numPlayers; i++) {
        // Place home planet with border distance into sphere.
        do {
            galaxy[i].x = rand() % (sphereDivisions[numPlayers][i][2] - (2 * homePlanetBorderDistance));      // random mod (maxX - 2*border)
            galaxy[i].x = galaxy[i].x + homePlanetBorderDistance + sphereDivisions[numPlayers][i][0];       // + border + minX
            galaxy[i].y = rand() % (sphereDivisions[numPlayers][i][3] - (2 * homePlanetBorderDistance));      // random mod (maxY - 2*border)
            galaxy[i].y = galaxy[i].y + homePlanetBorderDistance + sphereDivisions[numPlayers][i][1]; // + border + minY
        } while (occupyLocation(galaxy[i].x, galaxy[i].y));

        // Set home planet production.
        galaxy[i].prod = homePlanetProduction;

        // Set owner of home planet to current player.
        galaxy[i].owner = i+1;

        // Generate sphere of current player.
        for (j = 0; j < numWorldsPerSphere[i] - 1; j++) {
            // Calculate planet index.
            planetIndex = numPlayers + j + i * (numWorlds / numPlayers);

            // Set owner of planet to pirate.
            galaxy[planetIndex].owner = 0;

            // Randomly place planet into sphere.
            do {
                galaxy[planetIndex].x = rand() % sphereDivisions[numPlayers][i][2];      // random mod maxX
                galaxy[planetIndex].x = galaxy[planetIndex].x + sphereDivisions[numPlayers][i][0];       // + minX
                galaxy[planetIndex].y = rand() % sphereDivisions[numPlayers][i][3];      // random mod maxY
                galaxy[planetIndex].y = galaxy[planetIndex].y + sphereDivisions[numPlayers][i][1];       // + minY
            } while (occupyLocation(galaxy[planetIndex].x, galaxy[planetIndex].y));

            // Assign placed planet random amount of production based on sphereLimit.
            if (availableProduction[i] < 0){
                galaxy[planetIndex].prod = 0;
            } else{
                galaxy[planetIndex].prod = rand() % maxProduction;
                availableProduction[i] -= galaxy[planetIndex].prod;
            }

            // Give every planet their production as start ships.
            galaxy[planetIndex].ships = galaxy[planetIndex].prod;
        }
    }

    // Calculate initial ships of home planets based on production of three nearest pirate planets and amount of production in their sphere.
    for (i = 0; i < numPlayers; i++) {
        galaxy[i].ships = homePlanetInitialShips + (availableProduction[i] * excessProductionBonus); // TODO three closest

    }
}

/**
 * Initialises the game questions.
 */
void initGameInputs() {
    // Loop variables.
    int i;

    // Get number of players.
    numPlayers = getNumPlayers();

    // Get player names.
    for (i = 0; i < numPlayers; i++) {
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
    // Initialize list of planets to be updated.
    int i;
    int updateIndices[40];
    for(i = 0; i < 40; i++){
        if(i < numWorlds){
            updateIndices[i] = i;
        } else {
            updateIndices[i] = -1;
        }
    }

    // Plot start screen.
    //startScreen(); //TODO
    tgi_clear();

    // Handle initial questions.
    initGameInputs();
    tgi_clear();

    // Initialize everything that shouldn't be changed on the map.
    initGameGraphics();

    // Initialize world and map based on player acceptance.
    do {
        generateGalaxy();
        // TODO: redraw map empty!
        updateMap(&updateIndices, &galaxy);
    } while(!mapAcceptance());

    // Play the game until running out of years.
    while (year != totalYears) {
        // Fight & Updates Production mechanics of ships that should reach their destination in that year.
        // TODO defensive ships here!

        // Update map based on state.
        updateTable(&updateIndices, &galaxy, year);
        updateMap(&updateIndices, &galaxy);

        // Retrieve inputs of all players.
        retrieveInputs();
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