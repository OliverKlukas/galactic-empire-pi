#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
#include "graphics.h"
#include "../main.h"
#include "ranking.h"


/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/

/**
 * Prints the final ranking screen.
 *
 * @param numPlayer - Number of players.
 * @param playerNames - Sorted list of player names.
 * @param galaxyProduction - Sorted list of percentages of galaxy production ownership.
 * @param numberShips - Sorted list of total number of ships.
 */
void printCeremony(unsigned numPlayer, char **playerNames, unsigned *galaxyProduction, unsigned *numberShips) {
    // Loop variables.
    int i;
    char input;

    // Clear screen.
    clrscr();

    // Set colors for final screen.
    bgcolor(startBackgroundColor);
    textcolor(startTextColor);

    // Print first rows.
    cputs("FINAL STANDINGS:");
    gotoxy(0, 2);
    cputs("Place   Admiral   Prod   Ships");

    // Iterate over players.
    for (i = 0; i < numPlayer; i++) {
        gotoxy(0, 4 + i);
        cprintf("%d.      %s       %d      %d", i + 1, playerNames[i+1], galaxyProduction[i], numberShips[i]);
    }

    // Wait for f before finishing.
    gotoxy(0, 20);
    cprintf("The war is over, press f to finish!");
    while (input != 'f') {
        input = cgetc();
    }
}
