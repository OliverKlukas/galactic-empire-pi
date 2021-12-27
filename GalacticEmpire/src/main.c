#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <time.h>
#include "graphics.h"
#include "queue.h"


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
                {0, 0, 19,  9},              // 2 player
                {0, 10, 19, 19},
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
            galaxy[planetIndex].prod = rand() % maxProduction;
            if(availableProduction[i] < galaxy[planetIndex].prod){
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
 * Initialises the game questions.  // TODO: delete, can be used instead of typing in questions
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


// this function calculates the distance between two planets given by their indices in the galaxy struct
unsigned calcDistance(unsigned planetAIdx, unsigned planetBIdx)
{
    int xDistance;
    int yDistance;

    unsigned absXDistance;
    unsigned absYDistance;

    xDistance = galaxy[planetAIdx].x - galaxy[planetBIdx].x;
    absXDistance = (xDistance >= 0 ? xDistance:-xDistance); 

    yDistance = galaxy[planetAIdx].y - galaxy[planetBIdx].y;
    absYDistance = (yDistance >= 0 ? yDistance:-yDistance); 

    return absXDistance + absYDistance;
}


// Iterates all players and retrieves mission inputs and adds them to mission table
void retrieveInputsFromAllPlayers()
{
    int i;
    unsigned distance;
    unsigned spaceShipSpeed = 2; // should be made global
    unsigned timeToArrival;

    int* playerInputs; // retrieved inputs from player: [playerIter, origin, destination, nShips]

    for (i = 0; i < 4; i++)
    {
        playerInputs[i] = 0;
    }


    // todo: randomize player sequence
    for (i = 0; i < numPlayers; i++) 
    {
        while (1)
        {
            playerInputs = retrieveInputs(i + 1, playerNames[i], &galaxy, numWorlds);
            if (playerInputs[1] == -1)
            {
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

/*
 * Evaluates the mission array, triggers reinforcements and fights  
 */
void evaluateMissions()
{
    // todo:
    // for debugging: 
    int player, dest, nShips;
    
    int i;


    // test latest input from mission Table 
    for (i = 0; i < missionTable[year][0].size; i++) {
        player = dequeue(&missionTable[year][0]);
        dest = dequeue(&missionTable[year][1]);
        nShips = dequeue(&missionTable[year][2]);

        // if (galaxy[dest].owner == player || 1) // debugging
        if (galaxy[dest].owner == player)
        {
            galaxy[dest].ships += nShips;
            displayReinforcements(player, dest, nShips);
        }
        else
        {
            // fighting 
        }
        

        // for debugging
        /*
        printForDebugging("player", player);
        printForDebugging("dest", dest);
        printForDebugging("nShips", nShips);
        */
    }

}


/*
 * Updates the ships on all worlds in the galaxy array due to their production capabilities 
 */

void evaluateProduction()
{
    // todo: 
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
 * Main galactic empire game logic.
 */
void game() {
    // Plot start screen.
    startScreen();

    // Handle initial questions.
    //initGameInputs();

    // for debugging:
    initGameInputsMock();

    initializeMissionTable();

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

    // todo: to implement ! ... or not really necessary !
    
    // Play the game until running out of years.
    while (year <= totalYears) {
        // Fight & Updates Production mechanics of ships that should reach their destination in that year.
        // TODO defensive ships here!

        evaluateMissions();
        evaluateProduction();

        // Update map based on state, in the first round special treatment 
        updateTable(&galaxy, year); 
        updateMap(&galaxy);

        // Retrieve inputs of all players.
        retrieveInputsFromAllPlayers();
        year++;
        updateTable(&galaxy, year); 
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