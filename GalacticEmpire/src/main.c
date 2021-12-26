#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <time.h>
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
 * Shuffle array in-place.
 *
 * <p>Source: https://stackoverflow.com/questions/6127503/shuffle-array-in-c
 *
 * @param array - Reference to array.
 * @param n - Length of array.
 */
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
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
    unsigned maxProduction = 10;
    unsigned homePlanetProduction = 10;
    unsigned homePlanetBorderDistance[5] = {2, 2, 2, 2, 1};
    unsigned homePlanetInitialShips = 200;
    unsigned totalProductionCapacity = maxProduction * (numWorlds / 2);
    unsigned excessProductionBonus = 7;
    unsigned numWorldsPerSphere[5];
    unsigned piratePlanetInitialShipsFactor = 2;
    unsigned availableProduction[5];

    // Generate random order for players.
    int *players = (int*) malloc(numPlayers * sizeof(int));
    int player;

    // Divide galaxy into spheres depending on player number.
    for (i = 0; i < numPlayers; i++) {
        numWorldsPerSphere[i] = numWorlds / numPlayers;
        availableProduction[i] = totalProductionCapacity / numPlayers;
    }

    // Account for not fitting world numbers.
    if (((numWorlds / numPlayers) * numPlayers) < numWorlds) {
        numWorldsPerSphere[numPlayers - 1] += 1;
    }

    // Bring players in random order.
    for(i = 0; i < numPlayers; i++){
        players[i] = i;
    }
    shuffle(players, numPlayers);

    // Build all player spheres.
    for (i = 0; i < numPlayers; i++) {
        // Retrieve current random player.
        player = players[i];

        // Place home planet with border distance into sphere.
        do {
            galaxy[player].x = rand() % (sphereDivisions[numPlayers-1][i][2] - (2 * homePlanetBorderDistance[numPlayers-1]) - sphereDivisions[numPlayers-1][i][0]);
            galaxy[player].x = galaxy[player].x + homePlanetBorderDistance[numPlayers-1] + sphereDivisions[numPlayers-1][i][0];
            galaxy[player].y = rand() % (sphereDivisions[numPlayers-1][i][3] - (2 * homePlanetBorderDistance[numPlayers-1]) - sphereDivisions[numPlayers-1][i][1]);
            galaxy[player].y = galaxy[player].y + homePlanetBorderDistance[numPlayers-1] + sphereDivisions[numPlayers-1][i][1];
        } while (occupyLocation(galaxy[player].x, galaxy[player].y));

        // Set home planet production.
        galaxy[player].prod = homePlanetProduction;

        // Set owner of home planet to current player.
        galaxy[player].owner = player+1;

        // Generate sphere of current player.
        for (j = 0; j < numWorldsPerSphere[player] - 1; j++) {
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
            if (availableProduction[player] == 0){
                galaxy[planetIndex].prod = 0;
            } else{
                galaxy[planetIndex].prod = rand() % maxProduction;
                availableProduction[player] -= galaxy[planetIndex].prod;
            }

            // Give every planet their production as start ships.
            galaxy[planetIndex].ships = galaxy[planetIndex].prod * piratePlanetInitialShipsFactor;

            // Increment planet index.
            planetIndex++;
        }
    }

    // Calculate initial ships of home planets based amount of production in their sphere.
    for (i = 0; i < numPlayers; i++) {
        player = players[i];
        galaxy[player].ships = homePlanetInitialShips + (availableProduction[player] * excessProductionBonus);
    }

    free(players);
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
    // Plot start screen.
    //startScreen();  // TODO: comment in

    // Handle initial questions.
    initGameInputsMock(); // TODO delete: initGameInputsMock();

    // Initialize everything that shouldn't be changed on the map.
    initGameGraphics();

    // Initialize world and map based on player acceptance.
    generateGalaxy();
    updateMap(&galaxy);
    updateTable(&galaxy, year);
    while(!mapAcceptance()) {
        clearMap();
        generateGalaxy();
        updateMap(&galaxy);
        updateTable(&galaxy, year);
    }
    retrieveInputs();

    // Play the game until running out of years.
    while (year != totalYears) {
        // Fight & Updates Production mechanics of ships that should reach their destination in that year.
        // TODO defensive ships here!

        // Update map based on state.
        updateTable(&galaxy, year);
        updateMap(&galaxy);

        // Retrieve inputs of all players.
        retrieveInputs();
        year++;
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

    // Seed the random numbers arbitrary.
    srand(time(0));

    // Call main game function.
    game();

    // Reset the border.
    bordercolor(Border);

    printf("Done\n");
    return EXIT_SUCCESS;
}