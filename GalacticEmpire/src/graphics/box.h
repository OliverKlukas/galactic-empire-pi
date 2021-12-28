#ifndef GALACTIC_EMPIRE_PI_GAMEINTERACTIONS_H
#define GALACTIC_EMPIRE_PI_GAMEINTERACTIONS_H

/**
 *  Deletes current text in input field.
 */
void clearTextIOField();

/**
 *  Converts planet letter 'a', 'b' etc to index in galaxy struct 0 1 2.
 */
int convertPlanetLetterToNumberIndex(char planetLetter);

/**
 * Handles input of single planet letters.
 */
int readSinglePlanetLetter();

/**
 *  Reads number inputs.
 */
int readNumber();

/**
 * Retrieves the inputs of all players in one round.
 */
int *retrieveInputs();

/**
 * Displays reinforcements arriving at planets.
 */
void displayReinforcements(unsigned playerIter, unsigned worldIter, unsigned nShips);

/**
 * Simulates attack on a planet and updates the galaxy.
 */
void simulateFight();

/**
 * Display supernova information, special events.
 */
void supernova(unsigned world, unsigned numShips, unsigned player, char **allNames);

/**
 * Asks for acceptance of map.
 */
unsigned mapAcceptance();

#endif
