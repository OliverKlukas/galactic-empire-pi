#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include "main.h"


/*****************************************************************************/
/*                            Global variables                               */
/*****************************************************************************/


// Screen dimensions: these are the maximum pixel positions displayable
const unsigned maxX = 39;
const unsigned maxY = 24;

// Map settings.
int mapNLinesVertical = 20;
int mapNLinesHorizontal = 20;

int yearLineX = 0;
int yearLineY = 0;

int textLine1X = 0;
int textLine1Y = 0;

int textLine2X = 0;
int textLine2Y = 0;

int tableColumn1XMin = 0;
int tableColumn2XMin = 0;

int tableFirstRowYMin = 0;




// Number offset for plotting digits.
int numOffset = 48;

// Space between alphabet letters.
int letterSpacing = 8;

// Center 8x8 letters to coordinates.
int centerLetter = 4;

// Screen dimensions.
//unsigned maxX = 320; //TODO
//unsigned maxY = 200;

// Table coordinates.
unsigned tableCorner = 1; // TODO change all usages to this and delete below.

// Input box coordinates.


// Globally used color Palettes.
static const unsigned char GreenPalette[2] = {TGI_COLOR_GREEN, TGI_COLOR_WHITE};
static const unsigned char StandardPalette[7] = {TGI_COLOR_WHITE, TGI_COLOR_BLACK, TGI_COLOR_BLUE, TGI_COLOR_GREEN,
                                         TGI_COLOR_RED, TGI_COLOR_ORANGE, TGI_COLOR_YELLOW};

/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/

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
/*
void plotLetter(unsigned x, unsigned y, int letter, unsigned color) {

    // Variables to plot pixels.
    int i, j, plot;
    char *bitmap;

    // Receive bit config of letter with moder ascii.
    if (letter > 192 && letter < 219) {
        bitmap = font8x8_basic[letter - 128];
    } else if (letter > 64 && letter < 91) {
        bitmap = font8x8_basic[letter + 32];
    } else {
        bitmap = font8x8_basic[letter];
    }

    // Render letter bit by bit onto map.
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            plot = bitmap[i] & 1 << j;
            if (plot) {
                tgi_setcolor(color);
                tgi_setpixel(x + j, y + i);
            } else {
                tgi_setcolor(COLOR_BACK);
                tgi_setpixel(x + j, y + i);
            }
        }
    }
}
*/

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
/*
void plotText(unsigned x, unsigned y, char *sentence, unsigned color) {
    // Loop variables.
    int i;

    // Iterate over the sentence and plot each char.
    for (i = 0; sentence[i] != 0; i++) {
        plotLetter(x + i * letterSpacing, y, sentence[i], color);
    }
}
*/



/**
 * TODO: print and update the current user input
 */
// TODO: maybe later
void clearTextIOField()
{
    //tgi_setcolor(COLOR_BLACK);
    //tgi_bar(textLine1XMin, textLine1YMin, textLine2XMax, textLine2YMax);
}


// reads input string from current cursor position
/*char * getInputFrom()
{
    char Char;

    do {
        Char = cgetc ();
        strcat (str, Char);
    } while (Char != CH_ENTER);
    return str;
}
*/
// returns a 4 letter array 
int * retrieveInputs(int player) {
    static int inputs [4];
    return inputs;
    /*
    char origin;    
    char destination;
    unsigned nShips;

    // todo: make this somewhere else: 
    
    clearTextIOField();

    // 1st question
    plotText(textLine1XMin, textLine1YMin, "Admiral A:", 1);
    plotText(textLine2XMin, textLine2YMin, "Origin of fleet ?", 1);
    
    // Only the first inputted letter is read 
    origin = cgetc ();
    *while (cgetc () != CH_ENTER)
    {
        if (cgetc() == CH_SPACE || origin == CH_SPACE)
        {} 
    }

    // 2nd question
    clearTextIOField();
    plotText(textLine1XMin, textLine1YMin, "Destination ?", 1);
    destination = cgetc ();
    while (cgetc () != CH_ENTER);

    // 3rd question
    clearTextIOField();
    plotText(textLine1XMin, textLine1YMin, "# of Ships ?", 1);
    nShips = cgetc ();
    while (cgetc () != CH_ENTER);


    return inputs;
    */
}

/**
 * Places a letter on the map depending.
 *
 * @param xPos - x position in 20x20 raster.
 * @param yPos - y position in 20x20 raster.
 * @param letter - letter that should be placed.
 * @param palette - coloring.
 */
void placeLetterOnMap(unsigned xPos, unsigned yPos, int letter, unsigned color) {
     // TODO: LATER
     //plotLetter(xPos * (mapLineThickness + mapSquareSize) + margin + 1 + mapLineThickness / 2 - 4,
     //              yPos * (mapLineThickness + mapSquareSize) + margin + 1 + mapLineThickness / 2 - 4,
     //              letter, color);
}

/**
 * Updates the map given a list of indices of planets to be changed.
 *
 * @param indices - Integer list of length 40 while only indices >= 0 are evaluated.
 * @param galaxy - Current state of galaxy.
 */
void updateMap(int *indices, struct world *galaxy) {
    // Loop variable.
    int i;
    // TODO
    // Chosen color palette.
    //tgi_setpalette(StandardPalette);

    // Place to be updated worlds on map.
    for (i = 0; i < 40; i++) {
        // Check if map should be updated.
        if (indices[i] >= 0) {
            // Differentiate between small and capital letters.
            if (i < 20) {
                placeLetterOnMap(galaxy[i].x, galaxy[i].y, i + 65, galaxy[i].owner + 1);
            } else {
                placeLetterOnMap(galaxy[i].x, galaxy[i].y, i + 173, galaxy[i].owner + 1);
            }
        }
    }
}

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed graphics based on the global variables. Read only.
 * // TODO: similar to updateMap() we need a way to only update the worlds that were changed!
 */
void updateTable(unsigned year) {
    // Loop variables.
    int row, column;

    // Table text coordinates.
    char letter = 'a';
    //int tableTextX = (maxX / 2) + (2 * margin);
    //int tableTextY = margin + 2;

    // Chosen color palette.
    // tgi_setpalette(StandardPalette);

    // Update current year.
    // plotText(yearLineXMin, yearLineYMin, year, StandardPalette);

    /*
    if (year < 10) { // TODO: make a custom plot numbers 
        plotLetter(tableTextX + 5 * letterSpacing, maxY - (2 * margin), year + numOffset, COLOR_FORE);
    } else {
        plotLetter(tableTextX + 5 * letterSpacing, maxY - (2 * margin), (year / 10) + numOffset, COLOR_FORE);
        plotLetter(tableTextX + 6 * letterSpacing, maxY - (2 * margin), (year % 10) + numOffset, COLOR_FORE);
    }

    // Update first column of current planet occupations.
    for (column = 0; column < 2; column++) {
        for (row = 1; row < 22; row++) {
            // World.
            plotLetter(tableTextX, tableTextY + row * letterSpacing, letter, COLOR_FORE);
            letter += 1;

            // Production. // TODO: make a custom plot numbers
            plotLetter(tableTextX + 3 * letterSpacing, tableTextY + row * letterSpacing, 9 + numOffset,
                       COLOR_FORE);

            // Ships.
            plotLetter(tableTextX + 7 * letterSpacing, tableTextY + row * letterSpacing, 8 + numOffset,
                       COLOR_FORE);
        }
        tableTextX += 75;
        letter = 'A';
    }
    */
}


// anywhere, so usually map or table
void placeColoredLetter(int x, int y, char letter, int player)
{
    // TODO: to top: + rename 
    int textColorStandard = COLOR_WHITE;
    int playerColors[4] = {COLOR_GRAY1, COLOR_GRAY2, COLOR_WHITE, COLOR_BLUE};

    textcolor(playerColors[player]);

    cputcxy(x, y, letter);

    textcolor(textColorStandard);
}

/**
 * Initializes the standard game graphics. // todo: maybe it is a good idea to return the x y position of the year text Line and text field lines
 */
void initGameGraphics() {

    int i, j;

    int tableXMin = mapNLinesVertical;
    int tableXMax = maxX;
    int tableYMin = 0;
    int tableYMax = tableYMin + 20 + 3;

    int textFieldXMin = 0;
    int textFieldXMax = mapNLinesVertical - 1;
    int textFieldYMin = mapNLinesHorizontal;
    int textFieldYMax = maxY;


    clrscr();

    //// 1. Draw map grid
    // y
    for (i = 0; i < mapNLinesHorizontal; ++i)
    {
        // x
        for (j = 0; j < mapNLinesVertical; ++j)
        {
            if (j == 0 && i == 0)
            {
                cputcxy(j, i, CH_ULCORNER);
            }
            else if (j == mapNLinesVertical - 1 && i == 0)
            {
                cputcxy(j, i, CH_URCORNER);     
            }
            else if (i == 0)
            {
                cputcxy(j, i, CH_TTEE); 
            }
            else if (i == mapNLinesHorizontal - 1 && j == 0)
            {
                cputcxy(j, i, CH_LLCORNER); 
            }
            else if (i == mapNLinesHorizontal - 1 && j == mapNLinesVertical - 1)
            {
                cputcxy(j, i, CH_LRCORNER);
            }
            else if (j == 0)
            {
                cputcxy(j, i, CH_LTEE);
            }
            else if (i == mapNLinesHorizontal - 1)
            {
                cputcxy(j, i, CH_BTEE); 
            }
            else if (j == mapNLinesVertical - 1)
            {
                cputcxy(j, i, CH_RTEE);
            }
            else
            {
                cputcxy(j, i, CH_CROSS);
            }
        
        }
    }


    /// draw table
    // border
    cputcxy(tableXMin, tableYMin, CH_ULCORNER);
    chlinexy(tableXMin + 1, tableYMin, tableXMax - tableXMin - 1);
    cputcxy(tableXMax, tableYMin, CH_URCORNER);
    cvlinexy(tableXMax, tableYMin + 1, tableYMax - tableYMin - 1);
    cputcxy(tableXMax, tableYMax, CH_LRCORNER);
    chlinexy(tableXMin + 1, tableYMax, tableXMax - tableXMin - 1);
    cputcxy(tableXMin, tableYMax, CH_LLCORNER);
    cvlinexy(tableXMin, tableYMin + 1, tableYMax - tableYMin - 1);

    tableColumn1XMin = tableXMin + 1;

    // middle lines
    cvlinexy(tableXMin + (tableXMax - tableXMin)/2, tableYMin + 1, tableYMax - tableYMin - 1);
    cvlinexy(tableXMin + (tableXMax - tableXMin)/2 + 1, tableYMin + 1, tableYMax - tableYMin - 1);
    cputcxy(tableXMin + (tableXMax - tableXMin)/2, tableYMin, CH_TTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin)/2 + 1, tableYMin, CH_TTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin)/2, tableYMax, CH_BTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin)/2 + 1, tableYMax, CH_BTEE);

    tableColumn2XMin = tableXMin + (tableXMax - tableXMin)/2 + 2;

    // header
    cputsxy(tableXMin + 1, tableYMin + 1, "W Pr Shp");
    cputsxy(tableXMin + (tableXMax - tableXMin)/2 + 2, tableYMin + 1, "W Pr Shp");
    chlinexy(tableXMin + 1, tableYMin + 2, tableXMax - tableXMin - 1);
    cputcxy(tableXMin , tableYMin + 2, CH_LTEE);
    cputcxy(tableXMax , tableYMin + 2, CH_RTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin)/2, tableYMin + 2, CH_CROSS);
    cputcxy(tableXMin + (tableXMax - tableXMin)/2 + 1, tableYMin + 2, CH_CROSS);

    tableFirstRowYMin = tableYMin + 3;

    /// Year 
    yearLineX = tableXMin + 1;
    yearLineY = tableYMax + 1;
    cputsxy(yearLineX, yearLineY, "Year: 0");


    /// Input text field
    cputcxy(textFieldXMin, textFieldYMin, CH_ULCORNER);
    chlinexy(textFieldXMin + 1, textFieldYMin, textFieldXMax - textFieldXMin - 1);
    cputcxy(textFieldXMax, textFieldYMin, CH_URCORNER);
    cvlinexy(textFieldXMax, textFieldYMin + 1, textFieldYMax - textFieldYMin - 1);
    cputcxy(textFieldXMax, textFieldYMax, CH_LRCORNER);
    chlinexy(textFieldXMin + 1, textFieldYMax, textFieldXMax - textFieldXMin - 1);
    cputcxy(textFieldXMin, textFieldYMax, CH_LLCORNER);
    cvlinexy(textFieldXMin, textFieldYMin + 1, textFieldYMax - textFieldYMin - 1);
    
    textLine1X = textFieldXMin + 1;
    textLine1Y = textFieldYMin + 1;

    textLine2X = textLine1X;
    textLine2Y = textLine1Y + 1;

    // TODO: just testing map placement 
    placeColoredLetter(5, 0, 'a', 0);
    placeColoredLetter(5, 1, 'b', 1);
    placeColoredLetter(5, 2, 'c', 2);
    placeColoredLetter(5, 3, 'd', 3);

    // TODO: testing table placement
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin, 'a', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 1, 'b', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 2, 'c', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 3, 'd', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 4, 'e', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 5, 'f', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 6, 'g', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 7, 'h', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 8, 'i', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 9, 'j', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 10, 'k', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 11, 'l', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 12, 'm', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 13, 'n', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 14, 'o', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 15, 'p', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 16, 'q', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 17, 'r', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 18, 's', 0);
    placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + 19, 't', 0);
}




/**
 * Draws the start screen.
 *
 * <p> Waits for any button to be pressed to continue to the game.
 *
 */
void startScreen() {
    // Color background.
    /*
    tgi_setpalette(GreenPalette);

    // Write game description.
    plotText(maxX / 3, maxY / 2 - 15, "GALACTIC EMPIRE", COLOR_FORE);
    plotText(maxX / 3 + 4, (2 * maxY) / 3 - 15, "klukas edition", COLOR_FORE);

    // Wait for any button to be pressed.
    plotText(maxX / 3 + 15, maxY - 10, "press a key", COLOR_FORE);
    cgetc();
    */
}

/**
 * Retrieves the desired number of players.
 *
 * <p>Maximum number is 5.
 *
 * @return - Returns number of players.
 */
unsigned getNumPlayers() {
    int numPlayers = 1;
    /*
    // Color background.
    tgi_setpalette(GreenPalette);

    // Retrieve the number of players.
    while (numPlayers) {
        plotText(margin, margin, "How many players [1-5]?", COLOR_FORE);
        numPlayers = cgetc();
        numPlayers -= numOffset;
        plotLetter(200, margin, numPlayers + numOffset, COLOR_FORE);

        // Check if input is valid.
        if (numPlayers < 1 || numPlayers > 5) {
            plotText(margin, margin + 2 * letterSpacing, "The number of players needs to be 1-5!", COLOR_FORE);
            numPlayers = 1;
        } else {
            tgi_clear();
            return numPlayers;
        }
        tgi_clear();
    }
    */ 
    return numPlayers;
       
}

/**
 * Retrieves the name of a player.
 *
 * <p>Only names with 3 letters are accepted.
 *
 * @player - Number of player.
 * @return - Returns the name of the Player as string.
 */
char *getPlayerName(unsigned player) {
    int numChars = 0;
    return numChars;
    /*
    char name[3];

    // Plot the question.
    char *question = "Player number 0 is?";
    tgi_setpalette(GreenPalette);
    question[14] = player + 1 + '0';
    plotText(margin, margin, question, COLOR_FORE);

    // Retrieve a three letter name.
    while (numChars != 3) {
        name[numChars] = cgetc();
        plotLetter(165 + numChars * letterSpacing, margin, name[numChars], COLOR_FORE);
        // Check if input letter is valid.
        if ((name[numChars] > 192 && name[numChars] < 219) || (name[numChars] > 64 && name[numChars] < 91)) {
            numChars++;
        } else {
            plotText(margin, margin + 2 * letterSpacing, "Name needs to be 3 letters!", COLOR_FORE);
            tgi_clear();
            numChars = 0;
            plotText(margin, margin, question, COLOR_FORE);
        }
    }

    tgi_clear();
    return name; // TODO: adress of stack memory is returned, change to reference?
    */
}


/**
 * Retrieves the desired number of worlds.
 *
 * <p>Number of worlds needs to be between 10 and 40.
 *
 * @return - Returns number of worlds.
 */
unsigned getNumWorlds() {
    unsigned numWorlds = 0;
    unsigned numDigits = 0;
    int digit;
    return numWorlds;
    /*
    // Color background.
    tgi_setpalette(GreenPalette);

    // Plot question.
    plotText(margin, margin, "How many worlds [10-40]?", COLOR_FORE);

    // Retrieve the number of worlds.
    while (numDigits != 2) {
        digit = cgetc();
        plotLetter(205 + numDigits * letterSpacing, margin, digit, COLOR_FORE);
        digit -= numOffset;

        // Add digit to number.
        if (numDigits == 0) {
            numWorlds = numWorlds + (10 * digit);
        } else {
            numWorlds += digit;
        }

        // Check if input is valid.
        if (digit < 0 || digit > 9 || numWorlds > 40) {
            plotText(margin, margin + 2 * letterSpacing, "The number of worlds needs to be 10-40!", COLOR_FORE);
            tgi_clear();
            numDigits = 0;
            numWorlds = 0;
            plotText(margin, margin, "How many worlds [10-40]?", COLOR_FORE);
        } else {
            numDigits++;
        }
    }
    tgi_clear();
    return numWorlds;
    */
}


/**
 * Retrieves the desired number of years.
 *
 * <p>Number of years needs to be between 10 and 99.
 *
 * @return - Returns number of years.
 */
unsigned getYears() {
    unsigned numYears = 0;
    unsigned numDigits = 0;
    int digit;
    return numDigits;
    /*
    // Color background.
    tgi_setpalette(GreenPalette);

    // Plot question.
    plotText(margin, margin, "How many years in the game [10-99]?", COLOR_FORE);

    // Retrieve the number of worlds.
    while (numDigits != 2) {
        digit = cgetc();
        plotLetter(295 + numDigits * letterSpacing, margin, digit, COLOR_FORE);
        digit -= numOffset;

        // Add digit to number.
        if (numDigits == 0) {
            numYears = numYears + (10 * digit);
        } else {
            numYears += digit;
        }

        // Check if input is valid.
        if (digit < 0 || digit > 9 || numYears > 40) {
            plotText(margin, margin + 2 * letterSpacing, "The number of years needs to be 10-99!", COLOR_FORE);
            tgi_clear();
            numDigits = 0;
            numYears = 0;
            plotText(margin, margin, "How many years in the game [10-99]?", COLOR_FORE);
        } else {
            numDigits++;
        }
    }
    tgi_clear();
    return numYears;
    */
}

/**
 * Retrieve if defensive ships should be build on pirate planets.
 *
 * @return - Returns 1 for true or 0 for false.
 */
unsigned getDefensive() {
    return 0;
    /*
    char answer;

    // Color background.
    tgi_setpalette(GreenPalette);

    // Retrieve if defensive ships should be build.
    plotText(margin, margin, "Do neutral worlds have defenses [y/n]?", COLOR_FORE);
    answer = cgetc();

    // Check if input is valid.
    while (answer != 'y' && answer != 'n') {
        answer = cgetc();
    }

    // Plot answer.
    plotLetter(312, margin, answer, COLOR_FORE);

    // Clear the page and return the answer.
    tgi_clear();
    if (answer == 'y') {
        return 1;
    } else {
        return 0;
    }
    */
}

/**
* Retrieve if special events should occur.
 *
 * @return - Returns 1 for true or 0 for false.
*/
unsigned getEvents() {
    return 0;
    /*
    char answer;

    // Color background.
    tgi_setpalette(GreenPalette);

    // Retrieve if special events should occur.
    plotText(margin, margin, "Should special events occur [y/n]?", COLOR_FORE);

    // Check if input is valid.
    while (answer != 'y' && answer != 'n') {
        answer = cgetc();
    }

    // Plot answer.
    plotLetter(288, margin, answer, COLOR_FORE);

    // Clear the page and return the answer.
    tgi_clear();
    if (answer == 'y') {
        return 1;
    } else {
        return 0;
    }
    */
}

/**
* Asks for acceptance of map.
 *
 * @return - Returns 1 for not accepting and 0 for acceptance.
*/
unsigned mapAcceptance() {
    // Player acceptance;

    // gotoxy(textLine1X, textLine1Y);

    cputsxy(textLine1X, textLine1Y, "Would you like a");
    cputsxy(textLine2X, textLine2Y, "different map ?");
        
    return 0;
    /*
    char acceptance;

    // Color background.
    tgi_setpalette(StandardPalette);

    // Plot question.
    plotText(textLine1XMin,
             textLine1YMin,
             "Would you like a",
             COLOR_FORE);
    plotText(textLine2XMin,
             textLine2YMin,
             "different map?",
             COLOR_FORE);

    // Retrieve and check input.
    acceptance = cgetc();
    while (acceptance != 'y' && acceptance != 'n') {
        acceptance = cgetc();
    }

    // Return acceptance.
    if (acceptance == 'y') {
        return 0;
    } else {
        return 1;
    }

    // Erase input box. // TODO +1 for all the walls but make the corners a global thing.
    */
}
