#ifndef GALACTIC_EMPIRE_PI_GRAPHICS_H
#define GALACTIC_EMPIRE_PI_GRAPHICS_H

// Screen dimensions: these are the maximum pixel positions displayable.
extern const unsigned maxX;
extern const unsigned maxY;

// Map settings.
extern const int mapNLinesVertical;
extern const int mapNLinesHorizontal;

// Year settings.
extern int yearLineX;
extern int yearLineY;

// Text field coordinates.
extern int textLine1X;
extern int textLine1Y;
extern int textLine2X;
extern int textLine2Y;

// Table coordinates.
extern int tableColumn1XMin;
extern int tableColumn2XMin;
extern int tableFirstRowYMin;

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
