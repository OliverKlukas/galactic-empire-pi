#ifndef GALACTIC_EMPIRE_GCC_TESTS_MAIN_H
#define GALACTIC_EMPIRE_GCC_TESTS_MAIN_H

// Constants.
#define MAX_PLAYERS 5       // Maximum number of players.
#define MAX_PRODUCTION 10  // Maximum production of a world.
#define HOME_PLANET_PRODUCTION 10 // Production of home planet.
#define HOME_PLANET_INITIAL_SHIPS 200 // Initial ships on home planet.
#define EXCESS_PRODUCTION_BONUS 7 // Bonus production for excess production.
#define PIRATE_PRODUCTION_FACTOR 1 // Production factor for pirate planets.
#define MAX_WORLDS 40      // Maximum number of worlds.

// Struct definitions.
typedef struct {
    unsigned char x;             // 0-19 x axis position on map
    unsigned char y;             // 0-19 y axis position on map
    unsigned char owner;         // 0 is pirate, 1 - 5 players
    unsigned prod;          // 0 - x production
    unsigned ships;         // 0 - x number of ships
} world;

typedef struct {
    unsigned char x[MAX_WORLDS];    // 0-19 x axis position on map
    unsigned char y[MAX_WORLDS];    // 0-19 y axis position on map
    unsigned char owner[MAX_WORLDS];    // 0 is pirate, 1 - 5 players
    unsigned prod[MAX_WORLDS];     // 0 - x production
    unsigned ships[MAX_WORLDS];    // 0 - x number of ships
} Galaxy;

#endif
