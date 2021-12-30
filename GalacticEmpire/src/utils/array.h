#ifndef GALACTIC_EMPIRE_PI_ARRAY_H
#define GALACTIC_EMPIRE_PI_ARRAY_H

/**
 * Swaps two elements in an array.
 */
void swap(unsigned *xp, unsigned *yp);

/**
 * SelectionSort algorithm to sort winners.
 */
void sortRanking(unsigned ranking[], unsigned galaxyProduction[], unsigned numberShips[], unsigned n);

/**
 * Shuffle array in-place.
 */
void shuffle(unsigned *array, size_t n);

#endif
