/**
 * Places a character anywhere on the screen.
 *
 * <p>Write given letter bit by bit onto the screen while overwriting the content that was located at the given x and y.
 * The letters are 8x8 dimensioned and most ascii signs are supported.
 *
 * @param x - X coordinate of letter, upper left corner.
 * @param y - Y coordinate of letter, upper left corner.
 * @param letter - Letter to be plotted.
 */
void plotLetter(unsigned x, unsigned y, int letter);

/**
 * Writes a sentence on the screen.
 *
 * <p>Write given sentence bit by bit onto the screen while overwriting the content that was located at the given x and
 * y. The letters are 8x8 dimensioned and most ascii signs are supported. Distance between letters depends on spacing.
 *
 * @param x - Start X coordinate of sentence.
 * @param y - Start Y coordinate of sentence.
 * @param sentence - String Sentence to be plotted.
 */
void plotText(unsigned x, unsigned y, char *sentence);

/**
 * TODO: print all characters new or rather just the colors? Probably need to reprint them right?
 */
void updateMap();

/**
 * TODO: print and update the current user input
 */
void updateInput();

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
