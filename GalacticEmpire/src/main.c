#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <time.h>
#include "main.h"
#include "graphics/graphics.h"
#include "graphics/startscreen.h"
#include "graphics/box.h"
#include "graphics/map.h"
#include "graphics/questions.h"
#include "graphics/ranking.h"
#include "graphics/table.h"
#include "utils/queue.h"
#include "utils/array.h"


/*****************************************************************************/
/*                            Global variables                               */
/*****************************************************************************/


// Game specific variables.
unsigned numPlayers = 5;
unsigned numWorlds = 10;
unsigned year = 0;
unsigned totalYears = 0;
int defensiveShips = 1;
int events = 1;
char **playerNames;
world **galaxy;
Queue **missionTable;

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
                {0, 0, 19, 9},              // 2 player
                {0,  10, 19, 19},
                {0,  0,  0,  0},
                {0,  0,  0,  0},
                {0,  0, 0,  0}
        },
        {
                {5, 0, 14, 9},              // 3 player
                {0,  10, 9,  19},
                {10, 10, 19, 19},
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

// Scenario variables.
unsigned maxProduction = 10;
unsigned homePlanetProduction = 10;
unsigned homePlanetBorderDistance[5] = {2, 2, 2, 2, 1};
unsigned homePlanetInitialShips = 200;
unsigned excessProductionBonus = 7;
unsigned numWorldsPerSphere[5];
unsigned piratePlanetInitialShipsFactor = 1;
unsigned availableProduction[5];


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

    // Further scenario vars.
    unsigned totalProductionCapacity = maxProduction * (numWorlds / 2);

    // Divide galaxy into spheres depending on player number.
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
            galaxy[i]->x = rand()
                           % (sphereDivisions[numPlayers - 1][i][2]
                              - (2 * homePlanetBorderDistance[numPlayers - 1])
                              - sphereDivisions[numPlayers - 1][i][0]);
            galaxy[i]->x = galaxy[i]->x
                           + homePlanetBorderDistance[numPlayers - 1]
                           + sphereDivisions[numPlayers - 1][i][0];
            galaxy[i]->y = rand()
                           % (sphereDivisions[numPlayers - 1][i][3]
                              - (2 * homePlanetBorderDistance[numPlayers - 1])
                              - sphereDivisions[numPlayers - 1][i][1]);
            galaxy[i]->y = galaxy[i]->y
                           + homePlanetBorderDistance[numPlayers - 1]
                           + sphereDivisions[numPlayers - 1][i][1];
        } while (occupyLocation(galaxy[i]->x, galaxy[i]->y));

        // Set home planet production.
        galaxy[i]->prod = homePlanetProduction;

        // Set owner of home planet to current player.
        galaxy[i]->owner = i + 1;

        // Generate sphere of current player.
        for (j = 0; j < numWorldsPerSphere[i] - 1; j++) {
            // Set owner of planet to pirate.
            galaxy[planetIndex]->owner = 0;

            // Randomly place planet into sphere.
            do {
                galaxy[planetIndex]->x = rand() % (sphereDivisions[numPlayers - 1][i][2]
                                                   - sphereDivisions[numPlayers - 1][i][0]);
                galaxy[planetIndex]->x = galaxy[planetIndex]->x + sphereDivisions[numPlayers - 1][i][0];
                galaxy[planetIndex]->y = rand() % (sphereDivisions[numPlayers - 1][i][3]
                                                   - sphereDivisions[numPlayers - 1][i][1]);
                galaxy[planetIndex]->y = galaxy[planetIndex]->y + sphereDivisions[numPlayers - 1][i][1];
            } while (occupyLocation(galaxy[planetIndex]->x, galaxy[planetIndex]->y));

            // Assign placed planet random amount of production based on sphereLimit.
            galaxy[planetIndex]->prod = rand() % maxProduction;
            if (availableProduction[i] < galaxy[planetIndex]->prod) {
                galaxy[planetIndex]->prod = 0;
            } else {
                availableProduction[i] -= galaxy[planetIndex]->prod;
            }

            // Give every planet their production as start ships.
            galaxy[planetIndex]->ships = galaxy[planetIndex]->prod * piratePlanetInitialShipsFactor;

            // Increment planet index.
            planetIndex++;
        }
    }

    // Calculate initial ships of home planets based amount of production in their sphere.
    for (i = 0; i < numPlayers; i++) {
        galaxy[i]->ships = homePlanetInitialShips + (availableProduction[i] * excessProductionBonus);
    }
}

/**
 * Displays the final game stats and rankings.
 */
void awardCeremony() {
    // Loop variables.
    int i;

    // Winner ranking arrays.
    unsigned *galaxyProduction = calloc(numPlayers, sizeof(unsigned));
    unsigned *numberShips = calloc(numPlayers, sizeof(unsigned));
    unsigned *ranking = calloc(numPlayers, sizeof(unsigned));

    // Collect state of galaxy.
    for (i = 0; i < numWorlds; i++) {
        if (galaxy[i]->owner > 0) {
            // Total Production hold in galaxy.
            galaxyProduction[galaxy[i]->owner - 1] += galaxy[i]->prod;
            // Total number of ships per player and overall.
            numberShips[galaxy[i]->owner - 1] += galaxy[i]->ships;
        }
    }

    // Calculate achievements of players.
    for (i = 0; i < numPlayers; i++) {
        ranking[i] = galaxyProduction[i] + (numberShips[i] / maxProduction);
    }

    // Rank players.
    sortRanking(ranking, playerNames, galaxyProduction, numberShips, numPlayers);

    // Print out ranking of players.
    printCeremony(numPlayers, playerNames, galaxyProduction, numberShips);

    // Free allocated storage.
    free(galaxyProduction);
    free(numberShips);
    free(ranking);
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
    playerNames = (char **) malloc((numPlayers + 1) * sizeof(char *));
    playerNames[0] = (char *) malloc(4 * sizeof(char));
    playerNames[0] = "me";
    for (i = 0; i < numPlayers; i++) {
        playerNames[i + 1] = (char *) malloc(4 * sizeof(char));
        playerNames[i + 1] = getPlayerName(i);
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
 * Calculates the distance between two planets given by their indices in the galaxy struct.
 *
 * @param planetAIdx
 * @param planetBIdx
 * @return
 */
unsigned calcDistance(unsigned planetAIdx, unsigned planetBIdx) {
    int xDistance;
    int yDistance;
    unsigned absXDistance;
    unsigned absYDistance;
    xDistance = galaxy[planetAIdx]->x - galaxy[planetBIdx]->x;
    absXDistance = (xDistance >= 0 ? xDistance : -xDistance);
    yDistance = galaxy[planetAIdx]->y - galaxy[planetBIdx]->y;
    absYDistance = (yDistance >= 0 ? yDistance : -yDistance);
    return absXDistance + absYDistance;
}

/**
 * Iterates all players and retrieves mission inputs and adds them to mission table.
 */
void retrieveInputsFromAllPlayers() {
    int i;
    unsigned distance;
    unsigned spaceShipSpeed = 2;
    unsigned timeToArrival;
    unsigned *playerSequence = malloc(numPlayers * sizeof(unsigned));

    // retrieved inputs from player: [playerIter, origin, destination, nShips].
    int *playerInputs;
    for (i = 0; i < 4; i++) {
        playerInputs[i] = -1;
    }

    // Create randomized player sequence.
    for (i = 0; i < numPlayers; i++) {
        playerSequence[i] = i;
    }
    shuffle(playerSequence, numPlayers);

    // Retrieve missions of all players.
    for (i = 0; i < numPlayers; i++) {
        while (1) {
            playerInputs = retrieveInputs(playerSequence[i] + 1, playerNames[1 + playerSequence[i]], galaxy, numWorlds);
            if (playerInputs[1] == -1) {
                break;
            }

            // Calculate distance and arrival time.
            distance = calcDistance(playerInputs[1], playerInputs[2]);
            timeToArrival = distance / spaceShipSpeed;
            if (year + timeToArrival > totalYears) {
                break;
            } else if (timeToArrival < 1) {
                timeToArrival = 1;
            }

            // Add to mission table: global player index.
            enqueue(&missionTable[year + timeToArrival][0], playerInputs[0]);

            // Add to mission table: destination index.
            enqueue(&missionTable[year + timeToArrival][1], playerInputs[2]);

            // Add to mission table: number of ships.
            enqueue(&missionTable[year + timeToArrival][2], playerInputs[3]);
        }
    }
}

/**
 * Evaluates the mission array, triggers reinforcements and fights.
 */
void evaluateMissions() {
    int player, dest, nShips;
    int i;

    // Trigger supernova randomly with probability at x%.
    unsigned probSupernova = 5;
    unsigned superOccurrence;

    // Latest input from mission table.
    for (i = 0; i < missionTable[year][0].size; i++) {
        player = dequeue(&missionTable[year][0]);
        dest = dequeue(&missionTable[year][1]);
        nShips = dequeue(&missionTable[year][2]);

        // Let supernova occur randomly.
        superOccurrence = rand() % 100;
        if (events == 1 && superOccurrence < (probSupernova - 1)) {
            supernova(dest, nShips, player, playerNames);
            continue;
        }

        // Either reinforcements or a fight occurs.
        if (galaxy[dest]->owner == player) {
            galaxy[dest]->ships += nShips;
            displayReinforcements(player, dest, nShips);
        } else {
            simulateFight(galaxy, playerNames, dest, player, nShips);
        }

        // Update table on what's happening.
        updateTable(galaxy, numWorlds);
        updateMap(galaxy, numWorlds);
    }
}

/**
 * Initializes the mission table that holds all player game missions.
 */
void initializeMissionTable() {
    int i, j;

    // Maximum number of missions per year due to C64 memory restrictions.
    int capacity = 100;

    // Iteratively allocate mission table.
    missionTable = (Queue **) malloc(totalYears * sizeof(Queue *));
    for (i = 0; i < totalYears; ++i) {
        missionTable[i] = (Queue *) malloc(3 * sizeof(Queue));
        for (j = 0; j < 3; ++j) {
            missionTable[i][j].capacity = capacity;
            missionTable[i][j].front = missionTable[i][j].size = 0;
            missionTable[i][j].rear = capacity - 1;
            missionTable[i][j].array = (int *) malloc(missionTable[i][j].capacity * sizeof(int));
        }
    }
}

/**
 * Updates the ships on all worlds in the galaxy array due to their production capabilities.
 */
void updateProduction() {
    // Loop variables.
    int i;

    // Let whole galaxy produce its ships.
    for (i = 0; i < numWorlds; i++) {
        if (defensiveShips || galaxy[i]->owner != 0) {
            galaxy[i]->ships += galaxy[i]->prod;
        }
    }
}

/**
 * Main galactic empire game logic.
 */
void game() {
    // Loop variables.
    int i;

    // Plot start screen.
    startScreen();

    // Handle initial questions.
    initGameInputs();

    // Initialize mission matrix that manages trips.
    initializeMissionTable();

    // Allocate galaxy memory.
    galaxy = (world **) malloc(numWorlds * sizeof(world *));
    for (i = 0; i < numWorlds; i++) {
        galaxy[i] = (world *) calloc(1, sizeof(world));
    }

    // Initialize everything that shouldn't be changed on the map.
    initGameGraphics();

    // Initialize world and map based on player acceptance.
    generateGalaxy();
    updateTable(galaxy, numWorlds);
    updateMap(galaxy, numWorlds);
    updateYear(year);
    while (!mapAcceptance()) {
        clearMap();
        generateGalaxy();
        updateTable(galaxy, numWorlds);
        updateMap(galaxy, numWorlds);
    }

    // Play the game until running out of years.
    while (year <= totalYears) {
        // Fight & Updates Production mechanics of ships that should reach their destination in that year.
        evaluateMissions();

        // Update planets with their production.
        updateProduction();

        // Update map based on state, in the first round special treatment.
        updateTable(galaxy, numWorlds);
        updateMap(galaxy, numWorlds);

        // Retrieve inputs of all players.
        retrieveInputsFromAllPlayers();
        year++;
        updateYear(year);
    }

    // Final screen and award ceremony.
    awardCeremony();
}


/*****************************************************************************/
/*                                Main                                       */
/*****************************************************************************/


/**
 * Saves C64 colors and starts the game.
 *
 * @return Returns SUCCESS in case of completed game or FAILURE in case or errors.
 */
int main() {
    // Loop variables.
    int i;

    // Save the previous color to reset later on.
    unsigned char prevBorder, prevTextColor, prevBackgroundColor;

    // Set the colors for the start screen.
    prevBorder = bordercolor(COLOR_GREEN);
    prevBackgroundColor = bgcolor(COLOR_GREEN);
    prevTextColor = textcolor(COLOR_WHITE);

    // Seed the random numbers arbitrary.
    srand(time(0));

    // Call main game function.
    game();

    // Free globally allocated variables.
    for (i = 0; i < (numPlayers + 1); i++) {
        free(playerNames[i]);
    }
    free(playerNames);
    for (i = 0; i < totalYears + 1; i++) {
        free(missionTable[i]);
    }
    free(missionTable);
    for (i = 0; i < numWorlds; i++) {
        free(galaxy[i]);
    }
    free(galaxy);

    // Reset the colors.
    bordercolor(prevBorder);
    bgcolor(prevBackgroundColor);
    textcolor(prevTextColor);

    // Clear screen and finish.
    clrscr();
    printf("done.\n");
    return EXIT_SUCCESS;
}
