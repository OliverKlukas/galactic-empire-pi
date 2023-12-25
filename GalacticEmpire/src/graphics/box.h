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
int *retrieveInputs(unsigned char playerIter, char* playerName, Galaxy *galaxy, unsigned char numWorlds);

/**
 * Displays reinforcements arriving at planets.
 */
void displayReinforcements(unsigned char playerIter, unsigned char worldIter, unsigned nShips);

/**
 * Simulates attack on a planet and updates the galaxy.
 */
void simulateFight(Galaxy *galaxy, char *allNames, unsigned char world, unsigned char attacker, unsigned numShips);

/**
 * Display supernova information, special events.
 */
void supernova(unsigned char world, unsigned numShips, unsigned char player, char *allNames);

/**
 * Asks for acceptance of map.
 */
unsigned char mapAcceptance();

#endif
