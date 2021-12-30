#ifndef GALACTIC_EMPIRE_GCC_TESTS_MAIN_H
#define GALACTIC_EMPIRE_GCC_TESTS_MAIN_H

typedef struct {
    unsigned x;             // 0-19 x axis position on map
    unsigned y;             // 0-19 y axis position on map
    unsigned owner;         // 0 is pirate, 1 - 5 players
    unsigned prod;          // 0 - x production
    unsigned ships;         // 0 - x number of ships
} world;

#endif
