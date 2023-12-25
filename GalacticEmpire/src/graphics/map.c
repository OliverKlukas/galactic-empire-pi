#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
#include "graphics.h"
#include "../main.h"
#include "map.h"


/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/


/**
 * Updates the map with the current galaxy state.
 *
 * @param galaxy - Current state of galaxy.
 * @param numWorlds - Number of overall worlds.
 */
void updateMap(world **galaxy, unsigned numWorlds) {
    // Loop variable.
    int i;

    // Place to be updated worlds on map.
    for (i = 0; i < numWorlds; i++) {
        // Differentiate between small and capital letters.
        if (i < 20) {
            placeColoredLetter(galaxy[i]->x, galaxy[i]->y, i + 65, galaxy[i]->owner);
        } else {
            placeColoredLetter(galaxy[i]->x, galaxy[i]->y, i + 173, galaxy[i]->owner);
        }
    }
}

/**
 * Clears the map completely of entries.
 */
void clearMap() {
    // Loop variables.
    int i, j;

    // Draw map grid.
    for (i = 0; i < MAP_N_LINES_HORIZONTAL; ++i) {
        // x
        for (j = 0; j < MAP_N_LINES_VERTICAL; ++j) {
            if (j == 0 && i == 0) {
                cputcxy(j, i, CH_ULCORNER);
            } else if (j == MAP_N_LINES_VERTICAL - 1 && i == 0) {
                cputcxy(j, i, CH_URCORNER);
            } else if (i == 0) {
                cputcxy(j, i, CH_TTEE);
            } else if (i == MAP_N_LINES_HORIZONTAL - 1 && j == 0) {
                cputcxy(j, i, CH_LLCORNER);
            } else if (i == MAP_N_LINES_HORIZONTAL - 1 && j == MAP_N_LINES_VERTICAL - 1) {
                cputcxy(j, i, CH_LRCORNER);
            } else if (j == 0) {
                cputcxy(j, i, CH_LTEE);
            } else if (i == MAP_N_LINES_HORIZONTAL - 1) {
                cputcxy(j, i, CH_BTEE);
            } else if (j == MAP_N_LINES_VERTICAL - 1) {
                cputcxy(j, i, CH_RTEE);
            } else {
                cputcxy(j, i, CH_CROSS);
            }
        }
    }
}
