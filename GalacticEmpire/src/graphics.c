#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <tgi.h>
#include "font8x8_basic.h"

/*****************************************************************************/
/*                            Global variables                               */
/*****************************************************************************/


// Map settings.
int mapNLinesVertical = 15;
int mapNLinesHorizontal = 15;
int mapLineThickness = 1;
int mapSquareSize = 9;

// Outer margin of game.
int margin = 5;

// Number offset for plotting digits.
int numOffset = 48;

// Space between alphabet letters.
int letterSpacing = 8;

// Center 8x8 letters to coordinates.
int centerLetter = 4;

// Screen dimensions.
unsigned MaxX = 320;
unsigned MaxY = 200;

// Table coordinates.
unsigned tableCorner = 1; // TODO

// Input box coordinates.


// Globally used color Palettes.
static const unsigned char StandardPalette[2] = {TGI_COLOR_WHITE, TGI_COLOR_BLACK};
static const unsigned char GreenPalette[2] = {TGI_COLOR_GREEN, TGI_COLOR_WHITE};

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
void plotLetter(unsigned x, unsigned y, int letter, const unsigned char *palette) {

    // Variables to plot pixels.
    int i, j, plot;
    char *bitmap;

    // Chosen color palette.
    tgi_setpalette(palette);

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
                tgi_setcolor(1);
                tgi_setpixel(x + j, y + i);
            } else {
                tgi_setcolor(0);
                tgi_setpixel(x + j, y + i);
            }
        }
    }

    // Restore current foreground color.
    tgi_setcolor(1);
}

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
void plotText(unsigned x, unsigned y, char *sentence, const unsigned char *palette) {
    // Loop variables.
    int i;

    // Iterate over the sentence and plot each char.
    for (i = 0; sentence[i] != 0; i++) {
        plotLetter(x + i * letterSpacing, y, sentence[i], palette);
    }
}

/**
 * TODO: print and update the current user input
 */
void retrieveInputs() {

}

/**
 * TODO: only the worlds that were changed should be updated!
 */
void updateMap() {
    plotLetter(margin - centerLetter, margin - centerLetter, 'a', StandardPalette); // TODO figure out placement here
    plotLetter(margin - centerLetter + 2 * mapSquareSize, margin - centerLetter + 2 * mapSquareSize, 'c',
               StandardPalette);
    plotLetter(margin - centerLetter + 3 * mapSquareSize, margin - centerLetter + 2 * mapSquareSize, 'F',
               StandardPalette);
    plotLetter(margin - centerLetter, margin - centerLetter, 'b', StandardPalette);

}

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed graphics based on the global variables. Read only.
 * // TODO: similar to updateMap() we need a way to only update the worlds that were changed!
 */
void updateTable(unsigned MaxX, unsigned MaxY, unsigned year) {
    // Loop variables.
    int row, column;

    // Table text coordinates.
    char letter = 'a';
    int tableTextX = (MaxX / 2) + (2 * margin);
    int tableTextY = margin + 2;

    // Update current year.
    if (year < 10) { // TODO: make a custom plot numbers
        plotLetter(tableTextX + 5 * letterSpacing, MaxY - (2 * margin), year + numOffset, StandardPalette);
    } else {
        plotLetter(tableTextX + 5 * letterSpacing, MaxY - (2 * margin), (year / 10) + numOffset, StandardPalette);
        plotLetter(tableTextX + 6 * letterSpacing, MaxY - (2 * margin), (year % 10) + numOffset, StandardPalette);
    }

    // Update first column of current planet occupations.
    for (column = 0; column < 2; column++) {
        for (row = 1; row < 22; row++) {
            // World.
            plotLetter(tableTextX, tableTextY + row * letterSpacing, letter, StandardPalette);
            letter += 1;

            // Production. // TODO: make a custom plot numbers
            plotLetter(tableTextX + 3 * letterSpacing, tableTextY + row * letterSpacing, 9 + numOffset,
                       StandardPalette);

            // Ships.
            plotLetter(tableTextX + 7 * letterSpacing, tableTextY + row * letterSpacing, 8 + numOffset,
                       StandardPalette);
        }
        tableTextX += 75;
        letter = 'A';
    }
}

/**
 * Initializes the standard game graphics.
 */
void initGameGraphics(unsigned MaxX, unsigned MaxY) {
    // Loop parameters.
    int i, j, column;

    // Table text (X, Y) params.
    int tableTextX = (MaxX / 2) + (2 * margin);
    int tableTextY = margin + 2;

    // Table corner (X, Y) params.
    int tableUpperY = margin;
    int tableLowerY = MaxY - (3 * margin);
    int tableLeftX = (MaxX / 2) + margin;
    int tableRightX = MaxX - margin;

    // Input box corner (X, Y) params.
    int boxUpperY = mapNLinesHorizontal * (mapLineThickness + mapSquareSize) + mapLineThickness + (2 * margin);
    int boxLowerY = MaxY - margin;
    int boxLeftX = margin;
    int boxRightX = (MaxX / 2) - margin + 1;

    // Print the outer lines of the table.
    tgi_line(tableLeftX, tableUpperY, tableRightX, tableUpperY);
    tgi_line(tableLeftX, tableUpperY, tableLeftX, tableLowerY);
    tgi_line(tableLeftX, tableLowerY, tableRightX, tableLowerY);
    tgi_line(tableRightX, tableUpperY, tableRightX, tableLowerY);
    tgi_line((3 * MaxX / 4), tableUpperY, (3 * MaxX / 4), tableLowerY);

    // Print first row of the table.
    for (column = 0; column < 2; column++) {
        tableTextX += column * 75;
        plotLetter(tableTextX, tableTextY, 'W', StandardPalette);
        plotText(tableTextX + 2 * letterSpacing, tableTextY, "Pn", StandardPalette);
        plotText(tableTextX + 5 * letterSpacing, tableTextY, "Shp", StandardPalette);
    }

    // Print the game year.
    plotText((MaxX / 2) + (2 * margin), MaxY - (2 * margin), "Year:", StandardPalette);

    // Print map vertical lines.
    for (i = 0; i <= mapNLinesVertical; ++i) {
        for (j = 0; j < mapLineThickness; ++j) {
            tgi_line(i * (mapLineThickness + mapSquareSize) + j + margin, margin,
                     i * (mapLineThickness + mapSquareSize) + j + margin,
                     mapNLinesHorizontal * (mapLineThickness + mapSquareSize) + margin);
        }
    }

    // Print map horizontal lines.
    for (i = 0; i <= mapNLinesHorizontal; ++i) {
        for (j = 0; j < mapLineThickness; ++j) {
            tgi_line(margin, i * (mapLineThickness + mapSquareSize) + j + margin,
                     mapNLinesVertical * (mapLineThickness + mapSquareSize) + margin,
                     i * (mapLineThickness + mapSquareSize) + j + margin);
        }
    }

    // Print textual output box.
    tgi_line(boxLeftX, boxUpperY, boxRightX, boxUpperY);
    tgi_line(boxLeftX, boxUpperY, boxLeftX, boxLowerY);
    tgi_line(boxLeftX, boxLowerY, boxRightX, boxLowerY);
    tgi_line(boxRightX, boxUpperY, boxRightX, boxLowerY);
}

/**
 * Draws the start screen.
 *
 * <p> Waits for any button to be pressed to continue to the game.
 *
 * @param MaxX - Maximal unsigned Pixel value on x axis.
 * @param MaxY - Maximal unsigned Pixel value on y axis.
 */
void startScreen(unsigned MaxX, unsigned MaxY) {
    // Color background.
    tgi_setpalette(GreenPalette);

    // Write game description.
    plotText(MaxX / 3, MaxY / 2 - 15, "GALACTIC EMPIRE", GreenPalette);
    plotText(MaxX / 3 + 4, (2 * MaxY) / 3 - 15, "klukas edition", GreenPalette);

    // Wait for any button to be pressed.
    plotText(MaxX / 3 + 15, MaxY - 10, "press a key", GreenPalette);
    cgetc();
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

    // Retrieve the number of players.
    while (numPlayers) {
        plotText(margin, margin, "How many players [1-5]?", GreenPalette);
        numPlayers = cgetc();
        numPlayers -= numOffset;
        plotLetter(200, margin, numPlayers + numOffset, GreenPalette);

        // Check if input is valid.
        if (numPlayers < 1 || numPlayers > 5) {
            plotText(margin, margin + 2 * letterSpacing, "The number of players needs to be 1-5!", GreenPalette);
            numPlayers = 1;
        } else {
            tgi_clear();
            return numPlayers;
        }
        tgi_clear();
    }
}

/**
 * Retrieves the name of a player.
 *
 * <p>Only names with 3 letters are accepted.
 *
 * @player - Number of player.
 * @return - Returns the name of the Player as string.
 */
char *getPlayerName(unsigned player){
    int numChars = 0;
    char name[3];

    // Plot the question.
    char *question = "Player number 0 is?";
    question[14] = player+1 + '0';
    plotText(margin, margin, question, GreenPalette);

    // Retrieve a three letter name.
    while(numChars != 3){
        name[numChars] = cgetc();
        plotLetter(165 + numChars * letterSpacing, margin, name[numChars], GreenPalette);
        // Check if input letter is valid.
        if ((name[numChars] > 192 && name[numChars] < 219) || (name[numChars] > 64 && name[numChars] < 91)) {
            numChars++;
        } else {
            plotText(margin, margin + 2 * letterSpacing, "Name needs to be 3 letters!", GreenPalette);
            tgi_clear();
            numChars = 0;
            plotText(margin, margin, question, GreenPalette);
        }
    }

    tgi_clear();
    return name; // TODO: adress of stack memory is returned, change to reference?
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

    // Plot question.
    plotText(margin, margin, "How many worlds [10-40]?", GreenPalette);

    // Retrieve the number of worlds.
    while (numDigits != 2) {
        digit = cgetc();
        plotLetter(205 + numDigits * letterSpacing, margin, digit, GreenPalette);
        digit -= numOffset;

        // Add digit to number.
        if(numDigits == 0){
            numWorlds = numWorlds + (10*digit);
        } else{
            numWorlds += digit;
        }

        // Check if input is valid.
        if (digit < 0 || digit > 9 || numWorlds > 40 ) {
            plotText(margin, margin + 2 * letterSpacing, "The number of worlds needs to be 10-40!", GreenPalette);
            tgi_clear();
            numDigits = 0;
            numWorlds = 0;
            plotText(margin, margin, "How many worlds [10-40]?", GreenPalette);
        } else {
            numDigits++;
        }
    }
    tgi_clear();
    return numWorlds;
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

    // Plot question.
    plotText(margin, margin, "How many years in the game [10-99]?", GreenPalette);

    // Retrieve the number of worlds.
    while (numDigits != 2) {
        digit = cgetc();
        plotLetter(295 + numDigits * letterSpacing, margin, digit, GreenPalette);
        digit -= numOffset;

        // Add digit to number.
        if(numDigits == 0){
            numYears = numYears + (10*digit);
        } else{
            numYears += digit;
        }

        // Check if input is valid.
        if (digit < 0 || digit > 9 || numYears > 40 ) {
            plotText(margin, margin + 2 * letterSpacing, "The number of years needs to be 10-99!", GreenPalette);
            tgi_clear();
            numDigits = 0;
            numYears = 0;
            plotText(margin, margin, "How many years in the game [10-99]?", GreenPalette);
        } else {
            numDigits++;
        }
    }
    tgi_clear();
    return numYears;
}

/**
 * Retrieve if defensive ships should be build on pirate planets.
 *
 * @return - Returns 1 for true or 0 for false.
 */
unsigned getDefensive(){
    char answer;

    // Retrieve if defensive ships should be build.
    plotText(margin, margin, "Do neutral worlds have defenses [y/n]?", GreenPalette);
    answer = cgetc();

    // Check if input is valid.
    while (answer != 'y' && answer != 'n') {
        answer = cgetc();
    }

    // Plot answer.
    plotLetter(312, margin, answer, GreenPalette);

    // Clear the page and return the answer.
    tgi_clear();
    if(answer == 'y'){
        return 1;
    } else{
        return 0;
    }
}

/**
* Retrieve if special events should occur.
 *
 * @return - Returns 1 for true or 0 for false.
*/
unsigned getEvents(){
    char answer;

    // Retrieve if special events should occur.
    plotText(margin, margin, "Should special events occur [y/n]?", GreenPalette);

    // Check if input is valid.
    while (answer != 'y' && answer != 'n') {
        answer = cgetc();
    }

    // Plot answer.
    plotLetter(288, margin, answer, GreenPalette);

    // Clear the page and return the answer.
    tgi_clear();
    if(answer == 'y'){
        return 1;
    } else{
        return 0;
    }
}

/**
* Asks for acceptance of map.
 *
 * @return - Returns 1 for not accepting and 0 for acceptance.
*/
unsigned mapAcceptance(){
    // Player acceptance;
    char acceptance;

    // Plot question.
    plotText(2*margin,
             mapNLinesHorizontal * (mapLineThickness + mapSquareSize) + mapLineThickness + (3 * margin),
             "Would you like a",
             StandardPalette);
    plotText(2*margin,
             mapNLinesHorizontal * (mapLineThickness + mapSquareSize) + mapLineThickness + (3 * margin) + letterSpacing + 4,
             "different map [y/n]?",
             StandardPalette);

    // Retrieve and check input.
    acceptance = cgetc();
    while(acceptance != 'y' && acceptance != 'n'){
        acceptance = cgetc();
    }

    // Return acceptance.
    if(acceptance == 'y'){
        return 0;
    } else{
        return 1;
    }

    // Erase input box. // TODO +1 for all the walls but make the corners a global thing.
}