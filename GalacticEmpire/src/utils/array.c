#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <cc65.h>
#include <conio.h>
#include "array.h"


/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/


/**
 * Swaps two unsigned elements in an array.
 *
 * <p>Adapted from: https://www.geeksforgeeks.org/selection-sort/
 *
 * @param xp - Pointer to first element.
 * @param yp - Pointer to second element.
 */
void swap(unsigned *xp, unsigned *yp) {
    unsigned temp = *xp;
    *xp = *yp;
    *yp = temp;
}

/**
 * Swaps two string elements in an array.
 *
 * <p>Adapted from: https://www.geeksforgeeks.org/swap-strings-in-c/
 *
 * @param xp - Pointer to first element.
 * @param yp - Pointer to second element.
 */
void swapStrings(char *str1, char *str2) {
    char temp[4];
    strcpy(temp, str1);
    strcpy(str1, str2);
    strcpy(str2, temp);
}

/**
 * SelectionSort algorithm to sort winners.
 *
 * <p>Adapted from: https://www.geeksforgeeks.org/selection-sort/
 *
 * @param ranking - Ranking array that is used to calculate ranking.
 * @param galaxyProduction - Galaxy used to calculate production.
 * @param numberShips - Number of ships array.
 * @param n - Length of array.
 */
void sortRanking(unsigned ranking[], char *playerNames, unsigned galaxyProduction[], unsigned numberShips[], unsigned n) {
    unsigned i, j, min_idx;

    // One by one move boundary of unsorted subarray.
    for (i = 0; i < n - 1; ++i) {
        // Find the minimum element in unsorted array.
        min_idx = i;
        for (j = i + 1; j < n; ++j) {
            if (ranking[j] > ranking[min_idx]) {
                min_idx = j;
            }
        }

        // Swap the found minimum element with the first element for all arrays.
        swap(&ranking[min_idx], &ranking[i]);
        swap(&galaxyProduction[min_idx], &galaxyProduction[i]);
        swap(&numberShips[min_idx], &numberShips[i]);
        swapStrings(&playerNames[4 + min_idx*4], &playerNames[4 + i*4]);
    }
}

/**
 * Shuffle array in-place.
 *
 * <p>Source: https://stackoverflow.com/questions/6127503/shuffle-array-in-c
 *
 * @param array - Reference to array.
 * @param n - Length of array.
 */
void shuffle(unsigned *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; ++i) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            unsigned t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
