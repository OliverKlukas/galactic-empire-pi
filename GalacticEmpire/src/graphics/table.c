#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
#include "graphics.h"
#include "../main.h"
#include "table.h"


/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/


/**
 *  Deletes current text in table.
 */
void clearTable() {
    int i;
    for (i = 0; i < 20; i++) {
        cclearxy(tableColumn1XMin, tableFirstRowYMin + i, 8);
        cclearxy(tableColumn2XMin, tableFirstRowYMin + i, 8);
    }
}

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed tables graphics based on the global variables.
 * @param galaxy - Current state of galaxy.
 * @param year - Current game year.
 * @param numWorlds - Total years of game.
 */
void updateTable(world *galaxy, unsigned numWorlds) {
    // Loop variables.
    int i;

    // Clear table before update.
    clearTable();

    // Place to be updated worlds on map.
    for (i = 0; i < numWorlds; i++) {
        // Differentiate between first and second column.
        if (i < 20) {
            // Plot world name, production and ships.
            placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + i, i + 65, galaxy[i].owner);

            // Only show planets not owned by pirates.
            if (galaxy[i].owner != 0) {
                placeColoredNumber(tableColumn1XMin + 3, tableFirstRowYMin + i, galaxy[i].prod, galaxy[i].owner);
                placeColoredNumber(tableColumn1XMin + 7, tableFirstRowYMin + i, galaxy[i].ships, galaxy[i].owner);
            }
        } else {
            // Plot world name, production and ships.
            placeColoredLetter(tableColumn2XMin, tableFirstRowYMin + i - 20, i + 173, galaxy[i].owner);

            // Only show planets not owned by pirates.
            if (galaxy[i].owner != 0) {
                placeColoredNumber(tableColumn2XMin + 3, tableFirstRowYMin + i - 20, galaxy[i].prod, galaxy[i].owner);
                placeColoredNumber(tableColumn2XMin + 7, tableFirstRowYMin + i - 20, galaxy[i].ships, galaxy[i].owner);
            }
        }
    }
}

/**
 * Updates the current game year.
 */
void updateYear(unsigned year)
{
    // Update current year.
    gotoxy(yearLineX, yearLineY);
    cprintf("Year: %d", year);
}
