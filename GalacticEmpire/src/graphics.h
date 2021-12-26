/**
 * Places a character anywhere on the screen.
 *
 * @param x - X coordinate of letter, upper left corner.
 * @param y - Y coordinate of letter, upper left corner.
 * @param letter - Letter to be plotted.
 */
void plotLetter(unsigned x, unsigned y, int letter);

/**
 * Writes a sentence on the screen.
 *
 * @param x - Start X coordinate of sentence.
 * @param y - Start Y coordinate of sentence.
 * @param sentence - String Sentence to be plotted.
 */
void plotText(unsigned x, unsigned y, char *sentence);

/**
 * Updates the game map.
 */
void updateMap();

/**
 * Retrieves the inputs of all players in one round.
 */
void retrieveInputs();

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed graphics based on the global variables. Read only.
 */
void updateTable();

/**
 * Initializes the standard game graphics.
 */
void initGameGraphics();

/**
 * Plots the game initialisation screen.
 */
void startScreen();

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
 * Clears the map completely of entries.
 */
void clearMap();

/**
 * Retrieve if special events should occur.
 *
 * @return - Returns if special events should occur.
 */
unsigned getEvents();

/**
* Asks for acceptance of map.
 *
 * @return - Returns 1 for not accepting and 0 for acceptance.
*/
unsigned mapAcceptance();
