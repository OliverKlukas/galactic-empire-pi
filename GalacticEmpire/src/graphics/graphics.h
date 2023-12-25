#ifndef GALACTIC_EMPIRE_PI_GRAPHICS_H
#define GALACTIC_EMPIRE_PI_GRAPHICS_H

// Screen dimensions: these are the maximum pixel positions displayable
#define MAX_X 39
#define MAX_Y 24

// Map settings.
#define MAP_N_LINES_VERTICAL 20
#define MAP_N_LINES_HORIZONTAL 20

// Year settings.
#define YEAR_LINE_X 21
#define YEAR_LINE_Y 24

// Text field coordinates.
#define TEXT_LINE_1_X 1
#define TEXT_LINE_1_Y 21
#define TEXT_LINE_2_X 1
#define TEXT_LINE_2_Y 22

// Table coordinates.
#define TABLE_COLUMN_1_X_MIN 21
#define TABLE_COLUMN_2_X_MIN 31
#define TABLE_FIRST_ROW_Y_MIN 3

// Globally used color Palettes.
extern const unsigned startTextColor;
extern const unsigned startBackgroundColor;
extern const unsigned textColor;
extern const unsigned backgroundColor;
extern const unsigned playerColors[6];

/**
 * Places colored letter on screen.
 */
void placeColoredLetter(unsigned x, unsigned y, char character, int player);

/**
 *  Places colored number on screen.
 */
void placeColoredNumber(unsigned x, unsigned y, unsigned number, unsigned player);

/**
 * Initializes the standard game graphics.
 */
void initGameGraphics();

#endif
