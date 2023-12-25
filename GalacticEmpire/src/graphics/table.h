#ifndef GALACTIC_EMPIRE_PI_TABLE_H
#define GALACTIC_EMPIRE_PI_TABLE_H

/**
 *  Deletes current text in table.
 */
void clearTable();

/**
 * Draws the latest game table and year.
 */
void updateTable(world **galaxy, unsigned char numWorlds);

/**
 * Updates the current game year.
 */
void updateYear(unsigned char year);

#endif
