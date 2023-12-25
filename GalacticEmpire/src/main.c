#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <time.h>
#include <string.h>
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
unsigned char numPlayers = 5;
unsigned char numWorlds = 10;
unsigned char year = 0;
unsigned char totalYears = 0;
unsigned char defensiveShips = 1;
unsigned char events = 1;

char playerNames[4 * (MAX_PLAYERS + 1)];
world **galaxy;
Galaxy empire;  // TODO: support galaxies with less than MAX_PLANETS planets.
Queue **missionTable;

// Available planet locations.
unsigned char availableLocations[400];

// Available sphere divisions.
unsigned char sphereDivisions[5][5][4] = {
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
unsigned char homePlanetBorderDistance[5] = {2, 2, 2, 2, 1};
unsigned char numWorldsPerSphere[5];
unsigned availableProduction[5];

/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/


/**
 * Checks if a planet location is not already taken and occupies it if possible.
 *
 * @return - Returns 0 if available and 1 if not.
 */
unsigned occupyLocation(unsigned char x, unsigned char y) {
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
    unsigned char i, j;
    unsigned char planetIndex = numPlayers;

    // Further scenario vars.
    unsigned totalProductionCapacity = MAX_PRODUCTION * (numWorlds / 2);

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
            empire.x[i] = rand() % (sphereDivisions[numPlayers - 1][i][2]
                                    - (2 * homePlanetBorderDistance[numPlayers - 1])
                                    - sphereDivisions[numPlayers - 1][i][0]);
            empire.x[i] = empire.x[i]
                            + homePlanetBorderDistance[numPlayers - 1]
                            + sphereDivisions[numPlayers - 1][i][0];
            empire.y[i] = rand() % (sphereDivisions[numPlayers - 1][i][3]
                                    - (2 * homePlanetBorderDistance[numPlayers - 1])
                                    - sphereDivisions[numPlayers - 1][i][1]);
            empire.y[i] = empire.y[i]
                            + homePlanetBorderDistance[numPlayers - 1]
                            + sphereDivisions[numPlayers - 1][i][1];
        } while (occupyLocation(empire.x[i], empire.y[i]));

        // Set home planet production.
        empire.prod[i] = HOME_PLANET_PRODUCTION;

        // Set owner of home planet to current player.
        empire.owner[i] = i + 1;

        // Generate sphere of current player.
        for (j = 0; j < numWorldsPerSphere[i] - 1; j++) {
            // Set owner of planet to pirate.
            empire.owner[planetIndex] = 0;

            // Randomly place planet into sphere.
            do {
                empire.x[planetIndex] = rand() % (sphereDivisions[numPlayers - 1][i][2]
                                                   - sphereDivisions[numPlayers - 1][i][0]);
                empire.x[planetIndex] = empire.x[planetIndex] + sphereDivisions[numPlayers - 1][i][0];
                empire.y[planetIndex] = rand() % (sphereDivisions[numPlayers - 1][i][3]
                                                   - sphereDivisions[numPlayers - 1][i][1]);
                empire.y[planetIndex] = empire.y[planetIndex] + sphereDivisions[numPlayers - 1][i][1];
            } while (occupyLocation(empire.x[planetIndex], empire.y[planetIndex]));

            // Assign placed planet random amount of production based on sphereLimit.
            empire.prod[planetIndex] = rand() % MAX_PRODUCTION;
            if(availableProduction[i] < empire.prod[planetIndex]) {
                empire.prod[planetIndex] = 0;
            }
            else {
                availableProduction[i] -= empire.prod[planetIndex];
            }

            // Give every planet their production as start ships.
            empire.ships[planetIndex] = empire.prod[planetIndex] * PIRATE_PRODUCTION_FACTOR;

            // Increment planet index.
            planetIndex++;
        }
    }

    // Calculate initial ships of home planets based amount of production in their sphere.
    for (i = 0; i < numPlayers; i++) {
        empire.ships[i] = HOME_PLANET_INITIAL_SHIPS + (availableProduction[i] * EXCESS_PRODUCTION_BONUS);
    }
}

/**
 * Displays the final game stats and rankings.
 */
void awardCeremony() {
    // Loop variables.
    unsigned char i;

    // Winner ranking arrays.
    unsigned *galaxyProduction = calloc(numPlayers, sizeof(unsigned));
    unsigned *numberShips = calloc(numPlayers, sizeof(unsigned));
    unsigned *ranking = calloc(numPlayers, sizeof(unsigned));

    // Collect state of galaxy.
    for (i = 0; i < numWorlds; i++) {
        if(empire.owner[i] > 0) {
            // Total Production hold in galaxy.
            galaxyProduction[empire.owner[i] - 1] += empire.prod[i];
            // Total number of ships per player and overall.
            numberShips[empire.owner[i] - 1] += empire.ships[i];
        }
    }

    // Calculate achievements of players.
    for (i = 0; i < numPlayers; i++) {
        ranking[i] = galaxyProduction[i] + (numberShips[i] / MAX_PRODUCTION);
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
    char name[4];
    unsigned char i;

    // Get number of players.
    numPlayers = getNumPlayers();

    // Get player names.
    strcpy(playerNames, "Bot");
    for (i = 0; i < numPlayers; ++i) {
        getPlayerName(i, name);
        strcpy(playerNames + 4 * (i+1), name);
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
unsigned char calcDistance(unsigned char planetAIdx, unsigned char planetBIdx) {
    unsigned char xDistance;
    unsigned char yDistance;

    // Check if planets are the same.
    if(planetAIdx == planetBIdx) {
        return 0;
    }

    // Check which empire is further away in x and y direction to not overflow.f
    if(empire.x[planetAIdx] < empire.x[planetBIdx]) {
        xDistance = empire.x[planetBIdx] - empire.x[planetAIdx];
    } else {
        xDistance = empire.x[planetAIdx] - empire.x[planetBIdx];
    }
    if(empire.y[planetAIdx] < empire.y[planetBIdx]) {
        yDistance = empire.y[planetBIdx] - empire.y[planetAIdx];
    } else {
        yDistance = empire.y[planetAIdx] - empire.y[planetBIdx];
    }
    return xDistance + yDistance;
}

/**
 * Iterates all players and retrieves mission inputs and adds them to mission table.
 */
void retrieveInputsFromAllPlayers() {
    unsigned char i;
    unsigned char distance;
    unsigned char timeToArrival;
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
            playerInputs = retrieveInputs(playerSequence[i] + 1, &playerNames[4 + playerSequence[i]*4], galaxy, numWorlds); // TODO galaxy

            if (playerInputs[1] == -1) {
                break;
            }

            // Calculate distance and arrival time.
            distance = calcDistance(playerInputs[1], playerInputs[2]);
            timeToArrival = distance / 2;
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

            // Remove numShips from galaxy.
            empire.ships[playerInputs[1]] -= playerInputs[3];
        }
    }

    // Free allocated storage.
    free(playerSequence);
}

/**
 * Evaluates the mission array, triggers reinforcements and fights.
 */
void evaluateMissions() {
    int player, dest, nShips;
    unsigned char i;

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
        if(empire.owner[dest] == player) {
            empire.ships[dest] += nShips;
            displayReinforcements(player, dest, nShips);
        } else {
            simulateFight(galaxy, playerNames, dest, player, nShips); // TODO galaxy
        }

        // Update table on what's happening.
        updateTable(galaxy, numWorlds); // TODO galaxy
        updateMap(galaxy, numWorlds); // TODO galaxy
    }

    // Free mission table of passed year.
    free(missionTable[year]);   // TODO: there might be leaking memory with the array of the queue.
}

/**
 * Initializes the mission table that holds all player game missions.
 */
void initializeMissionTable() {
    unsigned char i, j;

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
    unsigned char i;

    // Let whole galaxy produce its ships.
    for (i = 0; i < numWorlds; i++) {
        if (defensiveShips || empire.owner[i] != 0) {
            empire.ships[i] += empire.prod[i];
        }
    }
}

/**
 * Clears the memory of the galaxy.
 */
void clearGalaxyMemory() {
    unsigned char i;
    for (i = 0; i < numWorlds; i++) {
        empire.x[i] = 0;
        empire.y[i] = 0;
        empire.prod[i] = 0;
        empire.ships[i] = 0;
        empire.owner[i] = 0;
    }
}

/**
 * Main galactic empire game logic.
 */
void game() {
    // Plot start screen.
    startScreen();

    // Handle initial questions.
    initGameInputs();

    // Initialize mission matrix that manages trips.
    initializeMissionTable();   // TODO fix it as one dimensional non malloc array.

    // Initialize everything that shouldn't be changed on the map.
    initGameGraphics();

    // Generate galaxy and update until map acceptance.
    updateYear(year);
    do {
        clearMap();
        generateGalaxy();
        updateTable(galaxy, numWorlds); // TODO galaxy
        updateMap(galaxy, numWorlds); // TODO galaxy
        if(mapAcceptance()){
            break;
        }
        clearGalaxyMemory();
    } while (1);

    // Play the game until running out of years.
    while (year <= totalYears) {
        // Fight & Updates Production mechanics of ships that should reach their destination in that year.
        evaluateMissions();

        // Update planets with their production.
        updateProduction();

        // Update map based on state, in the first round special treatment.
        updateTable(galaxy, numWorlds); // TODO galaxy
        updateMap(galaxy, numWorlds); // TODO galaxy

        // Retrieve inputs of all players.
        retrieveInputsFromAllPlayers();
        year++;
        updateYear(year);
        updateTable(galaxy, numWorlds); // TODO galaxy
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
    // Save the previous color to reset later on.
    unsigned char prevBorder, prevTextColor, prevBackgroundColor;

    // Set the colors for the start screen.
    prevBorder = bordercolor(COLOR_GREEN);
    prevBackgroundColor = bgcolor(COLOR_GREEN);
    prevTextColor = textcolor(COLOR_WHITE);

    // Seed the random numbers arbitrary.
    srand(time(0)); // TODO: this returns always 0, can we use pid to seed in vice?

    // Call main game function.
    game();

    // Free globally allocated variables.
    free(missionTable); // TODO: remove

    // Reset the colors.
    bordercolor(prevBorder);
    bgcolor(prevBackgroundColor);
    textcolor(prevTextColor);

    // Clear screen and finish.
    clrscr();
    printf("done.\n");
    return EXIT_SUCCESS;
}
