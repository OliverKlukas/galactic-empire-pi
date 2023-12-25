#ifndef GALACTIC_EMPIRE_PI_RANKING_H
#define GALACTIC_EMPIRE_PI_RANKING_H

/**
 * Prints the final ranking screen.
 *
 * @param numPlayer - Number of players.
 * @param playerNames - Sorted list of player names.
 * @param galaxyProduction - Sorted list of percentages of galaxy production ownership.
 * @param numberShips - Sorted list of total number of ships.
 */
void printCeremony(unsigned numPlayer, char *playerNames, unsigned *galaxyProduction, unsigned *numberShips);

#endif
