#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include "graphics.h"


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
    unsigned planetIndex = numPlayers;

    // Scenario variables.
    unsigned maxProduction = 20;
    unsigned homePlanetProduction = 10;
    unsigned homePlanetBorderDistance[5] = {2, 2, 2, 2, 1};
    unsigned homePlanetInitialShips = 200;
    unsigned totalProductionCapacity = maxProduction * (numWorlds / 2);
    unsigned excessProductionBonus = 7;

    // Available sphere divisions.
    unsigned sphereDivisions[5][5][4] = {
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
                    {5, 0, 14, 9},              // 3 player
                    {0,  10,  9,  19},
                    {10, 10,  19, 19},
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
            galaxy[i].x = rand() % (sphereDivisions[numPlayers-1][i][2] - (2 * homePlanetBorderDistance[numPlayers-1]) - sphereDivisions[numPlayers-1][i][0]);
            galaxy[i].x = galaxy[i].x + homePlanetBorderDistance[numPlayers-1] + sphereDivisions[numPlayers-1][i][0];
            galaxy[i].y = rand() % (sphereDivisions[numPlayers-1][i][3] - (2 * homePlanetBorderDistance[numPlayers-1]) - sphereDivisions[numPlayers-1][i][1]);
            galaxy[i].y = galaxy[i].y + homePlanetBorderDistance[numPlayers-1] + sphereDivisions[numPlayers-1][i][1];
        } while (occupyLocation(galaxy[i].x, galaxy[i].y));

        // Set home planet production.
        galaxy[i].prod = homePlanetProduction;

        // Set owner of home planet to current player.
        galaxy[i].owner = i+1;

        // Generate sphere of current player.
        for (j = 0; j < numWorldsPerSphere[i] - 1; j++) {
            // Set owner of planet to pirate.
            galaxy[planetIndex].owner = 0;

            // Randomly place planet into sphere.
            do {
                galaxy[planetIndex].x = rand() % (sphereDivisions[numPlayers-1][i][2] - sphereDivisions[numPlayers-1][i][0]);
                galaxy[planetIndex].x = galaxy[planetIndex].x + sphereDivisions[numPlayers-1][i][0];
                galaxy[planetIndex].y = rand() % (sphereDivisions[numPlayers-1][i][3] - sphereDivisions[numPlayers-1][i][1]);
                galaxy[planetIndex].y = galaxy[planetIndex].y + sphereDivisions[numPlayers-1][i][1];
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

            // Increment planet index.
            planetIndex++;
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
void initGameInputs() { // TODO: new conio in graphics
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
 * Initialises the game questions.  // TODO: delete
 */
void initGameInputsMock() {
    // Get number of players.
    numPlayers = 3;

    // Get player names.
    playerNames[0] = "Bas";
    playerNames[1] = "Oli";
    playerNames[2] = "Pit";

    // Get number of worlds.
    numWorlds = 40;

    // Get number of totalYears.
    totalYears = 25;

    // Get special events settings.
    defensiveShips = 1;
    events = 1;
}



/**
 * Main galactic empire game logic.
 */
void game() {
    // Initialize list of planets to be updated.
    int i;
    int indices[40];

    // Plot start screen.
    startScreen();

    // Handle initial questions.
    initGameInputs(); // TODO delete: initGameInputsMock();

    // Updates indices to update whole map.
    for(i = 0; i < 40; i++){
        if(i < numWorlds){
            indices[i] = 1;
        } else {
            indices[i] = -1;
        }
    }

    // Initialize everything that shouldn't be changed on the map.
    initGameGraphics();

    // Initialize world and map based on player acceptance.
    do {
        generateGalaxy();
        // TODO: redraw map empty!
        // updateMap(&indices, &galaxy); // TODO
    } while(!mapAcceptance());
    retrieveInputs();

    // Play the game until running out of years.
    while (year != totalYears) {
        // Fight & Updates Production mechanics of ships that should reach their destination in that year.
        // TODO defensive ships here!

        // Update map based on state.
        updateTable(&indices, &galaxy, year);
        updateMap(&indices, &galaxy);

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

    // Set the border colors.
    Border = bordercolor(COLOR_GREEN);

    // Call main game function.
    game();

    // Reset the border.
    bordercolor(Border);

    printf("Done\n");
    return EXIT_SUCCESS;
}