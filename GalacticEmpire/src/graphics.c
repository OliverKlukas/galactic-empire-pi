#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
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

// Year settings.
int yearLineX = 0;
int yearLineY = 0;

// Text field coordinates.
int textLine1X = 0;
int textLine1Y = 0;
int textLine2X = 0;
int textLine2Y = 0;

// Table coordinates.
int tableColumn1XMin = 0;
int tableColumn2XMin = 0;
int tableFirstRowYMin = 0;

// Globally used color Palettes.
const unsigned startTextColor = COLOR_WHITE;
const unsigned startBackgroundColor = COLOR_GREEN;
const unsigned textColor = COLOR_BLACK;
const unsigned backgroundColor = COLOR_WHITE;
const unsigned playerColors[6] = {COLOR_BLACK, COLOR_BLUE, COLOR_PURPLE, COLOR_GREEN, COLOR_RED, COLOR_BROWN};


/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/

/**
 * Places colored letter on screen.
 *
 * @param x - X coordinate on screen, 0-39.
 * @param y - Y coordinate on screen, 0-24.
 * @param character - Character that should be plotted.
 * @param player - Player that owns the character plotted, 0 = pirate, 1-5 = players.
 */
void placeColoredLetter(int x, int y, char character, int player) {
    textcolor(playerColors[player]);
    cputcxy(x, y, character);
    textcolor(textColor);
}

/**
 *  Places colored number on screen.
 *
 *  <p>Places the number with its right most x value. Numbers grow with its decimal places to the left.
 *
 * @param x - Right most X coordinate that the number can take, 0-39
 * @param y - Y coordinate on screen, 0-24.
 * @param number - Number that should be plotted.
 * @param player - Player that owns the character plotted, 0 = pirate, 1-5 = players.
 */
void placeColoredNumber(unsigned x, unsigned y, unsigned number, unsigned player) {
    // Copy variables.
    int digit;
    int num = number;
    int i = 0;

    // Set text color according to selected player.
    textcolor(playerColors[player]);

    // Plot number by breaking it down into its single digits.
    if (number == 0) {
        cputcxy(x, y, '0');
    } else {
        while (num > 0) {
            digit = num % 10;
            cputcxy(x - i, y, digit + '0');
            num = num / 10;
            i++;
        }
    }

    // Reset color to last used one.
    textcolor(textColor);
}

/**
 * Clears a line at given coordinate y.
 *
 * @param y - Line that should be cleared 0 - maxY.
 */
void clearLine(unsigned y) {
    gotoxy(0, y);
    cclear(maxX);
}

/**
 *  Deletes current text in input field.
 */
void clearTextIOField() {
    cclearxy(textLine1X, textLine1Y, mapNLinesVertical - 2);
    cclearxy(textLine2X, textLine2Y, mapNLinesVertical - 2);
}

// converts planet letter 'a', 'b' etc to index in galaxy struct 0 1 2 
int convertPlanetLetterToNumberIndex(char planetLetter) {
    if (planetLetter > 85) {
        return planetLetter - 173;
    }
    return planetLetter - 65;
}

int readSinglePlanetLetter() {
    /*
    *   1. What it should be: enter one character, press enter : returns int which is the index of the entered world in the galaxy struct 
    *   2. If more or less than one character, return -1: display wrong input in parent function
    *   3. If space is entered: return -2: to restart the entire input
    *   4. If only enter is pressed: -3 is returned 
    */

    char planet;
    char readChar;
    unsigned numChars = 0;

    do {
        readChar = cgetc();
        switch (readChar) {
            case CH_ENTER:
                // Check if three chars have been inputted.
                if (numChars == 0) {
                    // invalid input return 
                    return -3;
                } else {
                    // return fitting planet
                    return convertPlanetLetterToNumberIndex(planet);
                }
            case ' ':
                return -2;
            default:
                // Check if input is valid
                if (numChars < 1 && ((readChar >= 'a' && readChar <= 'z') || (readChar >= 'A' && readChar <= 'Z'))) {
                    planet = readChar;
                    numChars++;
                } else {
                    return -1;
                }
        }
    } while (readChar != CH_ENTER);
}

/*
*   1. What it should be: reads any kind of number entered, press enter : returns fitting int number
*   2. if any non digit is entered: return -1
    3. If nothing or 0 is entered: return -1 // 0 is handled by parent function
    4. If a number with more than 5 digits is entered: return -1
*   3. If space is entered: return -2: cancel the entire loop
*/
int readNumber() {
    int i;
    int readDigits[5] = {0, 0, 0, 0, 0};
    int multiplyBy[5] = {10000, 1000, 100, 10, 1};
    int number = 0;
    int nZerosReadIn = 0;
    char readChar;
    unsigned numChars = 0;

    do {
        readChar = cgetc();
        switch (readChar) {
            case CH_ENTER:
                // Check if three chars have been inputted.
                if (numChars == 0) {
                    // invalid input return 
                    return -1;
                } else {
                    // convert to int 
                    clearTextIOField();

                    // calc remaining zeros in read digits to convert to int number
                    nZerosReadIn = 5 - numChars;

                    // for debuggin:
                    /*
                    gotoxy(textLine1X, textLine1Y);
                    cprintf("nZerosReadIn: %01d", nZerosReadIn);
                    sleep(2);
                    */
                    for (i = 0; i < 5 - nZerosReadIn; ++i) {
                        number += multiplyBy[i + nZerosReadIn] * readDigits[i];
                    }
                    if (number == 0) {
                        return -1;
                    } else {
                        return number;
                    }
                }
            case ' ':
                return -2;
            default:
                // Check if input is valid
                if (numChars < 5 && isdigit(readChar)) {
                    readDigits[numChars] = readChar - '0';
                    numChars++;
                } else {
                    return -1;
                }
        }
    } while (readChar != CH_ENTER);


}


/**
 *  Deletes current text in table.
 */
void clearTable() {
    int i;
    for (i = 0; i < 20; i++) {
        cclearxy(tableColumn1XMin, tableFirstRowYMin + i, 8);
        cclearxy(tableColumn2XMin, tableFirstRowYMin + i, 8);
    }
}

/**
 * Retrieves inputs of game rounds.
 *
 * @param player - Current active player, unsigned 0 - (numPlayer-1)
 * @return - Returns integer array with [(player, origin, destination, ships), ...] that contains the inputs. returns [-1,-1,-1,-1] if the player is done putting in inputs and it is the next one's turn.
 */
int *retrieveInputs(int playerIter, char *playerName, struct world *galaxy, int numWorlds) {
    static int inputs[4];

    char readChar;
    int origin = -1;
    int destination = -1;
    int nShips = -1;

    while (1) {
        // 1st question: Origin 
        while (origin < 0) {
            textcolor(textColor); // todo: should already be defined somewhere else!
            cputsxy(textLine1X, textLine1Y, "Admiral ");
            textcolor(playerColors[playerIter]);
            cputs(playerName);
            textcolor(textColor);
            cputcxy(textLine1X + 11, textLine1Y, ':');
            cputsxy(textLine2X, textLine2Y, "Origin of fleet ?");

            origin = readSinglePlanetLetter();
            if (origin == -1 || (origin > (numWorlds - 1))) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Wrong input!");
                gotoxy(textLine2X, textLine2Y);
                cprintf("Origin Idx: %01d", origin);
                sleep(2);
                clearTextIOField();
            } else if (origin == -2) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Cancelled!");
                sleep(2);
                clearTextIOField();
            } else if (origin == -3) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Sure?");
                readChar = cgetc();
                if (readChar == CH_ENTER) {
                    origin = -1;
                    break;
                }
            } else if (!(galaxy[origin].owner == playerIter)) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Wrong input!");
                cputsxy(textLine2X, textLine2Y, "Not your world!");
                origin = -1;
                sleep(2);
                clearTextIOField();
            }
        }

        // for debugging:
        /*
        clearTextIOField();
        cputsxy(textLine1X, textLine1Y, "Success!");
        gotoxy(textLine2X, textLine2Y);
        cprintf("Origin Idx: %01d", origin);
        sleep(1);
        clearTextIOField();
        */

        // 2nd question: Destination
        while (destination < 0 && origin >= 0) {
            clearTextIOField();
            textcolor(textColor); // todo: should already be defined somewhere else!
            cputsxy(textLine1X, textLine1Y, "Admiral ");
            textcolor(playerColors[playerIter]);
            cputs(playerName);
            textcolor(textColor);
            cputcxy(textLine1X + 11, textLine1Y, ':');
            cputsxy(textLine2X, textLine2Y, "Destination ?");

            destination = readSinglePlanetLetter();
            if (destination == -1 || destination > numWorlds - 1) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Wrong input!");
                sleep(2);
                clearTextIOField();
            } else if (destination == -2) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Cancelled!");
                sleep(2);
                clearTextIOField();
                break;
            } else if (destination == origin) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Wrong input!");
                cputsxy(textLine2X, textLine2Y, "Origin = Dest!");
                sleep(2);
                clearTextIOField();

                destination = -1;
            } else {
                break;
            }
        }

        if (destination == -2) {
            origin = -1;
            continue;
        }

        // for debugging:
        /*
        clearTextIOField();
        cputsxy(textLine1X, textLine1Y, "Success!");
        gotoxy(textLine2X, textLine2Y);
        cprintf("Destination Idx: %01d", destination);
        sleep(1);
        clearTextIOField();
        */

        // 3rd Question: # ships:
        while (nShips < 0 && origin >= 0) {
            clearTextIOField();
            textcolor(textColor); // todo: should already be defined somewhere else!
            cputsxy(textLine1X, textLine1Y, "Admiral ");
            textcolor(playerColors[playerIter]);
            cputs(playerName);
            textcolor(textColor);
            cputcxy(textLine1X + 11, textLine1Y, ':');
            cputsxy(textLine2X, textLine2Y, "# Ships ?");

            nShips = readNumber();
            if (nShips == -1) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Wrong input!");
                sleep(2);
                clearTextIOField();
            } else if (nShips == -2) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Cancelled!");
                sleep(2);
                clearTextIOField();
                break;
            } else if (nShips > galaxy[origin].ships) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Wrong input!");
                cputsxy(textLine2X, textLine2Y, "Not enough ships!");
                sleep(2);
                clearTextIOField();
                nShips = -1;
            } else {
                break;
            }
        }
        if (nShips == -2) {
            origin = -1;
            destination = -1;
            continue;
        }

        // for debugging:
        /*
        clearTextIOField();
        cputsxy(textLine1X, textLine1Y, "Success!");
        gotoxy(textLine2X, textLine2Y);
        cprintf("nShips: %05d", nShips);
        sleep(1);
        clearTextIOField();
        */

        break;
    }

    inputs[0] = playerIter;
    inputs[1] = origin;
    inputs[2] = destination;
    inputs[3] = nShips;

    return inputs;

}

/**
 * Updates the map with the current galaxy state.
 *
 * @param galaxy - Current state of galaxy.
 */
void updateMap(struct world *galaxy) {
    // Loop variable.
    int i;

    // Place to be updated worlds on map.
    for (i = 0; i < 40; i++) {
        // Differentiate between small and capital letters.
        if (i < 20) {
            placeColoredLetter(galaxy[i].x, galaxy[i].y, i + 65, galaxy[i].owner);
        } else {
            placeColoredLetter(galaxy[i].x, galaxy[i].y, i + 173, galaxy[i].owner);
        }
    }
}

/**
 * Clears the map completely of entries.
 */
void clearMap() {
    // Loop variables.
    int i, j;

    // Draw map grid.
    for (i = 0; i < mapNLinesHorizontal; ++i) {
        // x
        for (j = 0; j < mapNLinesVertical; ++j) {
            if (j == 0 && i == 0) {
                cputcxy(j, i, CH_ULCORNER);
            } else if (j == mapNLinesVertical - 1 && i == 0) {
                cputcxy(j, i, CH_URCORNER);
            } else if (i == 0) {
                cputcxy(j, i, CH_TTEE);
            } else if (i == mapNLinesHorizontal - 1 && j == 0) {
                cputcxy(j, i, CH_LLCORNER);
            } else if (i == mapNLinesHorizontal - 1 && j == mapNLinesVertical - 1) {
                cputcxy(j, i, CH_LRCORNER);
            } else if (j == 0) {
                cputcxy(j, i, CH_LTEE);
            } else if (i == mapNLinesHorizontal - 1) {
                cputcxy(j, i, CH_BTEE);
            } else if (j == mapNLinesVertical - 1) {
                cputcxy(j, i, CH_RTEE);
            } else {
                cputcxy(j, i, CH_CROSS);
            }
        }
    }
}

/**
 * Draws the latest game table and year.
 *
 * <p>Updates the displayed tables graphics based on the global variables.
 * @param galaxy - Current state of galaxy.
 * @param year - Current game year.
 */
void updateTable(struct world *galaxy, unsigned year) {
    // Loop variables.
    int i;

    // Clear table before update.
    clearTable();

    // Update current year.
    gotoxy(yearLineX, yearLineY);
    cprintf("Year: %d", year);

    // Place to be updated worlds on map.
    for (i = 0; i < 40; i++) {
        // Differentiate between first and second column.
        if (i < 20) {
            // Plot world name, production and ships.
            placeColoredLetter(tableColumn1XMin, tableFirstRowYMin + i, i + 65, galaxy[i].owner);

            // Only show planets not owned by pirates.
            if (galaxy[i].owner != 0) {
                placeColoredNumber(tableColumn1XMin + 3, tableFirstRowYMin + i, galaxy[i].prod, galaxy[i].owner);
                placeColoredNumber(tableColumn1XMin + 7, tableFirstRowYMin + i, galaxy[i].ships, galaxy[i].owner);
            }
        } else {
            // Plot world name, production and ships.
            placeColoredLetter(tableColumn2XMin, tableFirstRowYMin + i - 20, i + 173, galaxy[i].owner);

            // Only show planets not owned by pirates.
            if (galaxy[i].owner != 0) {
                placeColoredNumber(tableColumn2XMin + 3, tableFirstRowYMin + i - 20, galaxy[i].prod, galaxy[i].owner);
                placeColoredNumber(tableColumn2XMin + 7, tableFirstRowYMin + i - 20, galaxy[i].ships, galaxy[i].owner);
            }
        }
    }
}

/**
 * Initializes the standard game graphics.
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

    // Set colors for the game.
    bgcolor(backgroundColor);
    textcolor(textColor);

    clrscr();
    //// 1. Draw map grid
    // y
    for (i = 0; i < mapNLinesHorizontal; ++i) {
        // x
        for (j = 0; j < mapNLinesVertical; ++j) {
            if (j == 0 && i == 0) {
                cputcxy(j, i, CH_ULCORNER);
            } else if (j == mapNLinesVertical - 1 && i == 0) {
                cputcxy(j, i, CH_URCORNER);
            } else if (i == 0) {
                cputcxy(j, i, CH_TTEE);
            } else if (i == mapNLinesHorizontal - 1 && j == 0) {
                cputcxy(j, i, CH_LLCORNER);
            } else if (i == mapNLinesHorizontal - 1 && j == mapNLinesVertical - 1) {
                cputcxy(j, i, CH_LRCORNER);
            } else if (j == 0) {
                cputcxy(j, i, CH_LTEE);
            } else if (i == mapNLinesHorizontal - 1) {
                cputcxy(j, i, CH_BTEE);
            } else if (j == mapNLinesVertical - 1) {
                cputcxy(j, i, CH_RTEE);
            } else {
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
    cvlinexy(tableXMin + (tableXMax - tableXMin) / 2, tableYMin + 1, tableYMax - tableYMin - 1);
    cvlinexy(tableXMin + (tableXMax - tableXMin) / 2 + 1, tableYMin + 1, tableYMax - tableYMin - 1);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2, tableYMin, CH_TTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2 + 1, tableYMin, CH_TTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2, tableYMax, CH_BTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2 + 1, tableYMax, CH_BTEE);

    tableColumn2XMin = tableXMin + (tableXMax - tableXMin) / 2 + 2;

    // header
    cputsxy(tableXMin + 1, tableYMin + 1, "W Pr Shp");
    cputsxy(tableXMin + (tableXMax - tableXMin) / 2 + 2, tableYMin + 1, "W Pr Shp");
    chlinexy(tableXMin + 1, tableYMin + 2, tableXMax - tableXMin - 1);
    cputcxy(tableXMin, tableYMin + 2, CH_LTEE);
    cputcxy(tableXMax, tableYMin + 2, CH_RTEE);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2, tableYMin + 2, CH_CROSS);
    cputcxy(tableXMin + (tableXMax - tableXMin) / 2 + 1, tableYMin + 2, CH_CROSS);
    tableFirstRowYMin = tableYMin + 3;

    /// Year 
    yearLineX = tableXMin + 1;
    yearLineY = tableYMax + 1;
    cputsxy(yearLineX, yearLineY, "Year:");

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
}

/**
 * Draws the start screen.
 *
 * <p>Waits for any button to be pressed to continue to the game.
 *
 */
void startScreen() {
    // Clear screen.
    clrscr();

    // Write game title.
    gotoxy(12, 10);
    cprintf("GALACTIC  EMPIRE");
    gotoxy(13, 14);
    cprintf("klukas edition");

    // Wait for any button to be pressed.
    gotoxy(15, 23);
    cprintf("press key");
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
    unsigned numPlayer = 0;
    char input;

    // Clear screen and go to start.
    clrscr();

    // Input question.
    cputs("How many players [1-5]? ");

    // Enable input cursor.
    cursor(1);

    // Retrieve number of players till enter is hit.
    do {
        input = cgetc();
        if (isdigit(input)) {
            // Check if input is a valid number.
            cputcxy(24, 0, input);
            numPlayer = input - '0';
        } else if (input == CH_DEL) {
            // Delete current input number.
            cclearxy(24, 0, 1);
            gotoxy(24, 0);
            numPlayer = 0;
        } else if (input == CH_ENTER && numPlayer > 0 && numPlayer < 6) {
            // No further input is expected.
            cursor(0);
        } else {
            // Warn player if wrong input format.
            cputsxy(0, 2, "The number of players needs to be 1-5!");
            sleep(2);
            clearLine(2);
            cclearxy(24, 0, 1);
            gotoxy(24, 0);
            input = ' ';
        }
    } while (input != CH_ENTER);
    return numPlayer;
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
    char input;
    char *name = malloc(4*sizeof (char));

    // Clear screen and go to start.
    clrscr();

    // Plot the question.
    cprintf("Player number %d is? ", player + 1);

    // Enable input cursor.
    cursor(1);

    // Retrieve player name.
    do {
        input = cgetc();
        switch (input) {
            case CH_ENTER:
                // Check if three chars have been inputted.
                if (numChars != 3) {
                    // Warn player if wrong input format.
                    cputsxy(0, 2, "Name needs to be 3 letters!");
                    sleep(2);
                    clearLine(2);
                    gotoxy(20 + numChars, 0);
                    input = ' ';
                } else {
                    cursor(0);
                }
                break;
            case CH_DEL:
                // Delete last character.
                switch (numChars) {
                    case 1:
                        cclearxy(20, 0, 1);
                        gotoxy(20, 0);
                        numChars--;
                        break;
                    case 2:
                        cclearxy(21, 0, 1);
                        gotoxy(21, 0);
                        numChars--;
                        break;
                    case 3:
                        cclearxy(22, 0, 1);
                        gotoxy(22, 0);
                        numChars--;
                }
                break;
            default:
                // Check if input is valid
                if (numChars < 3 && ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'))) {
                    name[numChars] = input;
                    cputcxy(20 + numChars, 0, input);
                    numChars++;
                } else {
                    // Warn player if wrong input format.
                    cputsxy(0, 2, "Name needs to be 3 letters!");
                    sleep(2);
                    clearLine(2);
                    gotoxy(20 + numChars, 0);
                }
        }
    } while (input != CH_ENTER);

    // Insert terminal symbol for string.
    name[3] = '\0';
    return name;
}

/**
 * Prints the final ranking screen.
 *
 * @param numPlayer - Number of players.
 * @param playerNames - Sorted list of player names.
 * @param galaxyProduction - Sorted list of percentages of galaxy production ownership.
 * @param numberShips - Sorted list of total number of ships.
 */
void printCeremony(unsigned numPlayer, char *playerNames[5], unsigned *galaxyProduction, unsigned *numberShips) {
    // Loop variables.
    int i;
    char input;

    // Clear screen.
    clrscr();

    // Set colors for final screen.
    bgcolor(startBackgroundColor);
    textcolor(startTextColor);

    // Print first rows.
    cputs("FINAL STANDINGS:");
    gotoxy(0, 2);
    cputs("Place   Admiral   Prod   Ships");

    // Iterate over players.
    for (i = 0; i < numPlayer; i++) {
        gotoxy(0, 4 + i);
        cprintf("%d.      %s       %d      %d", i + 1, playerNames[i], galaxyProduction[i], numberShips[i]);
    }

    // Wait for f before finishing.
    gotoxy(0, 20);
    cprintf("The war is over, press f to finish!");
    while (input != 'f') {
        input = cgetc();
    }
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
    char input;

    // Clear screen and go to start.
    clrscr();

    // Input question.
    cputs("How many worlds [10-40]? ");

    // Enable input cursor.
    cursor(1);

    // Retrieve number of players till enter is hit.
    do {
        input = cgetc();
        if (isdigit(input)) {
            // Differentiate per decimal place.
            switch (numDigits) {
                case 0:
                    cputcxy(25, 0, input);
                    numDigits++;
                    numWorlds = 10 * (input - '0');
                    break;
                case 1:
                    cputcxy(26, 0, input);
                    numDigits++;
                    numWorlds += input - '0';
                    break;
                default:
                    cputsxy(0, 2, "The number of worlds needs to be 10-40!");
                    sleep(2);
                    clearLine(2);
                    gotoxy(27, 0);
            }
        } else if (input == CH_DEL) {
            // Delete current input number.
            switch (numDigits) {
                case 1:
                    cclearxy(25, 0, 1);
                    gotoxy(25, 0);
                    numDigits = 0;
                    numWorlds = 0;
                    break;
                case 2:
                    cclearxy(26, 0, 1);
                    gotoxy(26, 0);
                    numDigits = 1;
                    numWorlds = numWorlds - (numWorlds % 10);
                    break;
            }
        } else if (input == CH_ENTER && numDigits == 2 && numWorlds > 9 && numWorlds < 41) {
            // No further input is expected.
            cursor(0);
        } else {
            // Warn player if wrong input format.
            cputsxy(0, 2, "The number of worlds needs to be 10-40!");
            sleep(2);
            clearLine(2);

            // Delete existing input.
            cclearxy(25, 0, 2);

            // Reset input field.
            numWorlds = 0;
            numDigits = 0;
            gotoxy(25, 0);
            input = ' ';
        }
    } while (input != CH_ENTER);
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
    char input;

    // Clear screen and go to start.
    clrscr();

    // Input question.
    cputs("How many years in the game [10-99]? ");

    // Enable input cursor.
    cursor(1);

    // Retrieve number of players till enter is hit.
    do {
        input = cgetc();
        if (isdigit(input)) {
            // Differentiate per decimal place.
            switch (numDigits) {
                case 0:
                    cputcxy(36, 0, input);
                    numDigits++;
                    numYears = 10 * (input - '0');
                    break;
                case 1:
                    cputcxy(37, 0, input);
                    numDigits++;
                    numYears += input - '0';
                    break;
                default:
                    cputsxy(0, 2, "The number of years needs to be 10-99!");
                    sleep(2);
                    clearLine(2);
                    gotoxy(38, 0);
            }
        } else if (input == CH_DEL) {
            // Delete current input number.
            switch (numDigits) {
                case 1:
                    cclearxy(36, 0, 1);
                    gotoxy(36, 0);
                    numDigits = 0;
                    numYears = 0;
                    break;
                case 2:
                    cclearxy(37, 0, 1);
                    gotoxy(37, 0);
                    numDigits = 1;
                    numYears = numYears - (numYears % 10);
                    break;
            }
        } else if (input == CH_ENTER && numDigits == 2 && numYears > 0 && numYears < 100) {
            // No further input is expected.
            cursor(0);
        } else {
            // Warn player if wrong input format.
            cputsxy(0, 2, "The number of years needs to be 10-99!");
            sleep(2);
            clearLine(2);

            // Delete existing input.
            cclearxy(36, 0, 2);

            // Reset input field.
            numYears = 0;
            numDigits = 0;
            gotoxy(36, 0);
            input = ' ';
        }
    } while (input != CH_ENTER);
    return numYears;
}

/**
 * Retrieve if defensive ships should be build on pirate planets.
 *
 * @return - Returns 1 for true or 0 for false.
 */
unsigned getDefensive() {
    unsigned answer = 2;
    char input;

    // Clear screen and go to start.
    clrscr();

    // Input question.
    cputs("Do neutral worlds have defense [y/n]? ");

    // Enable input cursor.
    cursor(1);

    // Retrieve number of players till enter is hit.
    do {
        input = cgetc();
        switch (input) {
            case 'y':
                cputcxy(38, 0, input);
                answer = 1;
                break;
            case 'n':
                cputcxy(38, 0, input);
                answer = 0;
                break;
            case CH_DEL:
                // Delete current answer.
                cclearxy(38, 0, 1);
                gotoxy(38, 0);
                answer = 2;
                break;
            case CH_ENTER:
                // Check if answer was entered.
                if (answer == 2) {
                    input = ' ';
                } else {
                    cursor(0);
                }
        }
    } while (input != CH_ENTER);
    return answer;
}

/**
* Retrieve if special events should occur.
 *
 * @return - Returns 1 for true or 0 for false.
*/
unsigned getEvents() {
    unsigned answer = 2;
    char input;

    // Clear screen and go to start.
    clrscr();

    // Input question.
    cputs("Should special events occur [y/n]? ");

    // Enable input cursor.
    cursor(1);

    // Retrieve number of players till enter is hit.
    do {
        input = cgetc();
        switch (input) {
            case 'y':
                cputcxy(35, 0, input);
                answer = 1;
                break;
            case 'n':
                cputcxy(35, 0, input);
                answer = 0;
                break;
            case CH_DEL:
                // Delete current answer.
                cclearxy(35, 0, 1);
                gotoxy(35, 0);
                answer = 2;
                break;
            case CH_ENTER:
                // Check if answer was entered.
                if (answer == 2) {
                    input = ' ';
                } else {
                    cursor(0);
                }
        }
    } while (input != CH_ENTER);
    return answer;
}

/**
* Asks for acceptance of map.
 *
 * @return - Returns 1 for not accepting and 0 for acceptance.
*/
unsigned mapAcceptance() {

    unsigned answer = 2;
    char input;

    // Player acceptance;
    cputsxy(textLine1X, textLine1Y, "Would you like a");
    cputsxy(textLine2X, textLine2Y, "different map?");

    // Enable input cursor.
    gotoxy(textLine2X + 15, textLine2Y);
    cursor(1);

    // Retrieve number of players till enter is hit.
    do {
        input = cgetc();
        switch (input) {
            case 'y':
                cputcxy(textLine2X + 15, textLine2Y, input);
                answer = 0;
                break;
            case 'n':
                cputcxy(textLine2X + 15, textLine2Y, input);
                answer = 1;
                break;
            case CH_DEL:
                // Delete current answer.
                cclearxy(textLine2X + 15, textLine2Y, 1);
                gotoxy(textLine2X + 15, textLine2Y);
                answer = 2;
                break;
            case CH_ENTER:
                // Check if answer was entered.
                if (answer == 2) {
                    input = ' ';
                } else {
                    cursor(0);
                }
        }
    } while (input != CH_ENTER);

    // erase text IO field 
    clearTextIOField();

    return answer;
}
