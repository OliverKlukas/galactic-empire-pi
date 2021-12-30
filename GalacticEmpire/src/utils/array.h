#ifndef GALACTIC_EMPIRE_PI_ARRAY_H
#define GALACTIC_EMPIRE_PI_ARRAY_H

/**
 * Swaps two unsigned elements in an array.
 */
void swap(unsigned *xp, unsigned *yp);

/**
 * Swaps two string elements in an array.
 */
void swapStrings(char *xp, char *yp);

/**
 * SelectionSort algorithm to sort winners.
 */
void sortRanking(unsigned ranking[], char **playerNames, unsigned galaxyProduction[], unsigned numberShips[], unsigned n);

/**
 * Shuffle array in-place.
 */
void shuffle(unsigned *array, size_t n);

#endif
