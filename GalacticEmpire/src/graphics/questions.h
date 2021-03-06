#ifndef GALACTIC_EMPIRE_PI_QUESTIONS_H
#define GALACTIC_EMPIRE_PI_QUESTIONS_H

/**
 * Clears a line at given coordinate y.
 *
 * @param y - Line that should be cleared 0 - maxY.
 */
void clearLine(unsigned y);

/**
 * Retrieve the number of players.
 *
 * @return - Returns the number of players retrieved from the input.
 */
unsigned getNumPlayers();

/**
 * Retrieve a player name.
 *
 * @param player - Number of respective player.
 * @return - Returns the name of the player as a String.
 */
char *getPlayerName(unsigned player);

/**
 * Retrieve the number of worlds.
 *
 * @return - Returns the number of worlds set by the player.
 */
unsigned getNumWorlds();

/**
 * Retrieve the number of years.
 *
 * @return - Returns the number of years set by the player.
 */
unsigned getYears();

/**
 * Retrieve if defensive ships should be build on pirate planets.
 *
 * @return - Returns if defensive ships should be build by pirates.
 */
unsigned getDefensive();

/**
 * Retrieve if special events should occur.
 *
 * @return - Returns if special events should occur.
 */
unsigned getEvents();

#endif
