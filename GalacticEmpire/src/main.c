#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <time.h>
#include "graphics/graphics.h"
#include "graphics/startscreen.h"
#include "graphics/box.h"
#include "graphics/map.h"
#include "graphics/questions.h"
#include "graphics/ranking.h"
#include "graphics/table.h"
#include "utils/queue.h"


/*****************************************************************************/
/*                            Custom datatypes                               */
/*****************************************************************************/


typedef struct {
    unsigned x;             // 0-19 x axis position on map
    unsigned y;             // 0-19 y axis position on map
    unsigned owner;         // 0 is pirate, 1 - 5 players
    unsigned prod;          // 0 - x production
    unsigned ships;         // 0 - x number of ships
} world;


/*****************************************************************************/
/*                            Global variables                               */
/*****************************************************************************/


// Game specific variables.
unsigned numPlayers = 5;
char **playerNames;
unsigned numWorlds = 10;
unsigned year = 0;
unsigned totalYears = 0;
int defensiveShips = 1;
int events = 1;
//world *galaxy;
world galaxy[40];
Queue **missionTable;
char **names; // should be identical to playerNames

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
 * Shuffle array in-place.
 *
 * <p>Source: https://stackoverflow.com/questions/6127503/shuffle-array-in-c
 *
 * @param array - Reference to array.
 * @param n - Length of array.
 */
void shuffle(unsigned *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            unsigned t = array[j];
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
            galaxy[i].x = rand() %
                          (sphereDivisions[numPlayers - 1][i][2] - (2 * homePlanetBorderDistance[numPlayers - 1]) -
                           sphereDivisions[numPlayers - 1][i][0]);
            galaxy[i].x =
                    galaxy[i].x + homePlanetBorderDistance[numPlayers - 1] + sphereDivisions[numPlayers - 1][i][0];
            galaxy[i].y = rand() %
                          (sphereDivisions[numPlayers - 1][i][3] - (2 * homePlanetBorderDistance[numPlayers - 1]) -
                           sphereDivisions[numPlayers - 1][i][1]);
            galaxy[i].y =
                    galaxy[i].y + homePlanetBorderDistance[numPlayers - 1] + sphereDivisions[numPlayers - 1][i][1];
        } while (occupyLocation(galaxy[i].x, galaxy[i].y));

        // Set home planet production.
        galaxy[i].prod = homePlanetProduction;

        // Set owner of home planet to current player.
        galaxy[i].owner = i + 1;

        // Generate sphere of current player.
        for (j = 0; j < numWorldsPerSphere[i] - 1; j++) {
            // Set owner of planet to pirate.
            galaxy[planetIndex].owner = 0;

            // Randomly place planet into sphere.
            do {
                galaxy[planetIndex].x =
                        rand() % (sphereDivisions[numPlayers - 1][i][2] - sphereDivisions[numPlayers - 1][i][0]);
                galaxy[planetIndex].x = galaxy[planetIndex].x + sphereDivisions[numPlayers - 1][i][0];
                galaxy[planetIndex].y =
                        rand() % (sphereDivisions[numPlayers - 1][i][3] - sphereDivisions[numPlayers - 1][i][1]);
                galaxy[planetIndex].y = galaxy[planetIndex].y + sphereDivisions[numPlayers - 1][i][1];
            } while (occupyLocation(galaxy[planetIndex].x, galaxy[planetIndex].y));

            // Assign placed planet random amount of production based on sphereLimit.
            galaxy[planetIndex].prod = rand() % maxProduction;
            if (availableProduction[i] < galaxy[planetIndex].prod) {
                galaxy[planetIndex].prod = 0;
            } else {
                availableProduction[i] -= galaxy[planetIndex].prod;
            }

            // Give every planet their production as start ships.
            galaxy[planetIndex].ships = galaxy[planetIndex].prod * piratePlanetInitialShipsFactor;

            // Increment planet index.
            planetIndex++;
        }
    }

    // Calculate initial ships of home planets based amount of production in their sphere.
    for (i = 0; i < numPlayers; i++) {
        galaxy[i].ships = homePlanetInitialShips + (availableProduction[i] * excessProductionBonus);
    }
}

/**
 * Swap two elements in an array.
 *
 * <p>Adapted from: https://www.geeksforgeeks.org/selection-sort/
 *
 * @param xp - Pointer to first element.
 * @param yp - Pointer to second element.
 */
void swap(unsigned *xp, unsigned *yp) {
    unsigned temp = *xp;
    *xp = *yp;
    *yp = temp;
}

/**
 * SelectionSort algorithm to sort winners.
 *
 * <p>Adapted from: https://www.geeksforgeeks.org/selection-sort/
 *
 * @param ranking - Ranking array that is used to calculate ranking.
 * @param galaxyProduction - Galaxy used to calculate production.
 * @param numberShips - Number of ships array.
 * @param n - Length of array.
 */
void sortRanking(unsigned ranking[], unsigned galaxyProduction[], unsigned numberShips[], unsigned n) {
    unsigned i, j, min_idx;

    // One by one move boundary of unsorted subarray.
    for (i = 0; i < n - 1; i++) {
        // Find the minimum element in unsorted array.
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (ranking[j] > ranking[min_idx]) {
                min_idx = j;
            }
        }

        // Swap the found minimum element with the first element for all arrays.
        swap(&ranking[min_idx], &ranking[i]);
        swap(&galaxyProduction[min_idx], &galaxyProduction[i]);
        swap(&numberShips[min_idx], &numberShips[i]);
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
        if (galaxy[i].owner > 0) {
            // Total Production hold in galaxy.
            galaxyProduction[galaxy[i].owner - 1] += galaxy[i].prod;
            // Total number of ships per player and overall.
            numberShips[galaxy[i].owner - 1] += galaxy[i].ships;
        }
    }

    // Calculate achievements of players.
    for (i = 0; i < numPlayers; i++) {
        ranking[i] = galaxyProduction[i] + (numberShips[i] / maxProduction);
    }

    // Rank players.
    sortRanking(ranking, galaxyProduction, numberShips, numPlayers);

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
    playerNames = (char **) malloc(numPlayers * sizeof(char *));
    for (i = 0; i < numPlayers; i++) {
        playerNames[i] = (char *) malloc(4 * sizeof(char));
        playerNames[i] = getPlayerName(i);
    }

    // Get number of worlds.
    numWorlds = getNumWorlds();

    // Get number of totalYears.
    totalYears = getYears();

    // Get special events settings.
    defensiveShips = getDefensive();
    events = getEvents();

    // again player names with "me" ==> probably redundant //TODO
    // Allocate array with all names.
    names = (char **) malloc((numPlayers+1) * sizeof(char *));
    names[0] = (char *) malloc(4 * sizeof(char));
    names[0] = "me";
    for (i = 0; i < numPlayers; i++) {
        names[i+1] = (char *) malloc(4 * sizeof(char));
        names[i+1] = playerNames[i];
    }
}

/**
 * This function calculates the distance between two planets given by their indices in the galaxy struct.
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

    xDistance = galaxy[planetAIdx].x - galaxy[planetBIdx].x;
    absXDistance = (xDistance >= 0 ? xDistance : -xDistance);

    yDistance = galaxy[planetAIdx].y - galaxy[planetBIdx].y;
    absYDistance = (yDistance >= 0 ? yDistance : -yDistance);

    return absXDistance + absYDistance;
}

/**
 * Iterates all players and retrieves mission inputs and adds them to mission table.
 */
void retrieveInputsFromAllPlayers() {
    int i;
    unsigned distance;
    unsigned spaceShipSpeed = 2; // should be made global
    unsigned timeToArrival;
    unsigned *playerSequence = malloc(numPlayers * sizeof(unsigned));

    int* playerInputs; // retrieved inputs from player: [playerIter, origin, destination, nShips]

    for (i = 0; i < 4; i++)
    {
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
            playerInputs = retrieveInputs(playerSequence[i] + 1, playerNames[playerSequence[i]], &galaxy, numWorlds);
            if (playerInputs[1] == -1) {
                break;
            }

            // calc distance and arrival time 
            distance = calcDistance(playerInputs[1], playerInputs[2]);
            timeToArrival = distance/spaceShipSpeed;
            
            if (year + timeToArrival > totalYears)
            {
                break;
            } else if (timeToArrival < 1){
                timeToArrival = 1;
            }
    
            // add to mission table
            // timeToArrival = 1; // debugging 
            enqueue(&missionTable[year+timeToArrival][0], playerInputs[0]); // global player index
            enqueue(&missionTable[year+timeToArrival][1], playerInputs[2]); // destination index
            enqueue(&missionTable[year+timeToArrival][2], playerInputs[3]); // number of ships
        }
    }
}

/**
 * Evaluates the mission array, triggers reinforcements and fights.
 */
void evaluateMissions()
{
    int player, dest, nShips;
    int i;
    
    // Trigger supernova randomly.
    unsigned probSupernova = 5;         // Supernova probability at x%
    unsigned superOccurrence;

   

    // test latest input from mission Table 
    for (i = 0; i < missionTable[year][0].size; i++) {
        player = dequeue(&missionTable[year][0]);
        dest = dequeue(&missionTable[year][1]);
        nShips = dequeue(&missionTable[year][2]);

        // Let supernova occur randomly.
        superOccurrence = rand() % 100;
        if(events == 1 && superOccurrence < (probSupernova - 1)){
            supernova(dest, nShips, player, names);
            continue;
        }

        // if (galaxy[dest].owner == player || 1) // debugging
        if (galaxy[dest].owner == player)
        {
            galaxy[dest].ships += nShips;
            displayReinforcements(player, dest, nShips);
        }
        else
        {
            // fighting
            simulateFight(&galaxy, names, dest, player, nShips);
        }
        
        // for debugging
        /*
        printForDebugging("player", player);
        printForDebugging("dest", dest);
        printForDebugging("nShips", nShips);
        */
    }
}

void initializeMissionTable()
{
    int i, j;
    int capacity = 100; // a maximum of 500 missions per 

    missionTable = (Queue **)malloc(totalYears * sizeof(Queue *));
    for (i = 0; i < totalYears; ++i)
    {
        missionTable[i] = (Queue *)malloc(3 * sizeof(Queue));
        for (j = 0; j < 3; ++j)
        {
            missionTable[i][j].capacity = capacity;
            missionTable[i][j].front = missionTable[i][j].size = 0;
            missionTable[i][j].rear = capacity - 1;
            missionTable[i][j].array = (int*)malloc(missionTable[i][j].capacity * sizeof(int));
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
        if(defensiveShips || galaxy[i].owner != 0){
            galaxy[i].ships += galaxy[i].prod;
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
    /*galaxy = (world *)malloc(numWorlds * sizeof(world));
    for(i = 0; i < numWorlds; i++){
        galaxy[i].x = 0;
        galaxy[i].y = 0;
        galaxy[i].prod = 0;
        galaxy[i].owner = 0;
        galaxy[i].ships = 0;
    }
    gotoxy(0, 3); // TODO
    cprintf("%d",galaxy[0].x);
    cprintf("%d",galaxy[1].x);
    cprintf("%d",galaxy[2].x);
    cprintf("%d",galaxy[3].x);
    cprintf("%d",galaxy[4].x);
    cgetc();*/

    // Initialize everything that shouldn't be changed on the map.
    initGameGraphics();

    // Initialize world and map based on player acceptance.
    generateGalaxy();
    updateMap(&galaxy, numWorlds);
    updateTable(&galaxy, year, numWorlds);
    updateYear(year);
    while (!mapAcceptance()) {
        clearMap();
        generateGalaxy();
        updateMap(&galaxy, numWorlds);
        updateTable(&galaxy, year, numWorlds);
    }

    // Play the game until running out of years.
    while (year <= totalYears) {
        // Fight & Updates Production mechanics of ships that should reach their destination in that year.
        evaluateMissions();

        // Update planets with their production.
        updateProduction();

        // Update map based on state, in the first round special treatment.
        updateTable(&galaxy, year, numWorlds);
        updateMap(&galaxy, numWorlds);

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
 * Checks for driver compatibility and starts the Game
 *
 * <p>Partially adapted from: https://github.com/cc65/cc65/blob/master/samples/tgidemo.c
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
    for (i = 0; i < numPlayers; i++) { // TODO remove one names
        free(playerNames[i]);
    }
    free(playerNames);
    for (i = 0; i < numPlayers+1; i++) {
        free(names[i]);
    }
    free(names);
    for (i = 0; i < totalYears+1; i++) {
        free(missionTable[i]);
    }
    free(missionTable);
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