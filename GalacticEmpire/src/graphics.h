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
 * Updates the map with the current galaxy state.
 *
 * @param galaxy - Current state of galaxy.
 * @param numWorlds - Number of overall worlds.
 */
void updateMap();


/**
 * Simulates attack on a planet and updates the galaxy.
 *
 * @param allNames - All player names 1-x and Me at 0.
 * @param world - World that's under attack.
 * @param attacker - Integer value of attacker player.
 * @param numShips - Number of ships the attacker is attacking with.
 */
void simulateFight();

/**
 * Display supernova information, special events.
 *
 * @param world - Destination where the ships should have gone to.
 * @param numShips - Number of ships lost.
 * @param player - Player who lost the ships as string.
 * @param allNames - All player names 1-x and Me at 0.
 */
void supernova(unsigned world, unsigned numShips, unsigned player, char **allNames);

/**
 * Retrieves the inputs of all players in one round.
 */
int *retrieveInputs();

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed graphics based on the global variables.
 *
 * @param galaxy - Current state of galaxy.
 * @param year - Current game year.
 * @param numWorlds - Total years of game.
 */
void updateTable();

/**
 * Prints the final ranking screen.
 *
 * @param numPlayer - Number of players.
 * @param playerNames - Sorted list of player names.
 * @param galaxyProduction - Sorted list of percentages of galaxy production ownership.
 * @param numberShips - Sorted list of total number of ships.
 */
void printCeremony(unsigned numPlayer, char *playerNames[5], unsigned *galaxyProduction, unsigned *numberShips);

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
