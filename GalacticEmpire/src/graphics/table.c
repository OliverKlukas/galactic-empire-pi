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
    unsigned char i;
    for (i = 0; i < 20; i++) {
        cclearxy(TABLE_COLUMN_1_X_MIN, TABLE_FIRST_ROW_Y_MIN + i, 8);
        cclearxy(TABLE_COLUMN_2_X_MIN, TABLE_FIRST_ROW_Y_MIN + i, 8);
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
void updateTable(world **galaxy, unsigned char numWorlds) {
    // Loop variables.
    unsigned char i;

    // Clear table before update.
    clearTable();

    // Place to be updated worlds on map.
    for (i = 0; i < numWorlds; i++) {

        // Differentiate between first and second column.
        if (i < 20) {
            // Plot world name, production and ships.
            placeColoredLetter(TABLE_COLUMN_1_X_MIN, TABLE_FIRST_ROW_Y_MIN + i, i + 65, galaxy[i]->owner);

            // Only show planets not owned by pirates.
            if (galaxy[i]->owner != 0) {
                placeColoredNumber(TABLE_COLUMN_1_X_MIN + 3, TABLE_FIRST_ROW_Y_MIN + i, galaxy[i]->prod, galaxy[i]->owner);
                placeColoredNumber(TABLE_COLUMN_1_X_MIN + 7, TABLE_FIRST_ROW_Y_MIN + i, galaxy[i]->ships, galaxy[i]->owner);
            }
        } else {
            // Plot world name, production and ships.
            placeColoredLetter(TABLE_COLUMN_2_X_MIN, TABLE_FIRST_ROW_Y_MIN + i - 20, i + 173, galaxy[i]->owner);

            // Only show planets not owned by pirates.
            if (galaxy[i]->owner != 0) {
                placeColoredNumber(TABLE_COLUMN_2_X_MIN + 3, TABLE_FIRST_ROW_Y_MIN + i - 20, galaxy[i]->prod, galaxy[i]->owner);
                placeColoredNumber(TABLE_COLUMN_2_X_MIN + 7, TABLE_FIRST_ROW_Y_MIN + i - 20, galaxy[i]->ships, galaxy[i]->owner);
            }
        }
    }
}

/**
 * Updates the current game year.
 */
void updateYear(unsigned char year)
{
    // Update current year.
    gotoxy(YEAR_LINE_X, YEAR_LINE_Y);
    cprintf("Year: %d", year);
}
