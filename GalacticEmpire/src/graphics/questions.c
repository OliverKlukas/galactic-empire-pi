#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
#include "graphics.h"
#include "questions.h"


/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/


/**
 * Clears a line at given coordinate y.
 *
 * @param y - Line that should be cleared 0 - MAX_Y.
 */
void clearLine(unsigned y) {
    gotoxy(0, y);
    cclear(MAX_X);
}

/**
 * Retrieves the desired number of players.
 *
 * <p>Maximum number is 5.
 *
 * @return - Returns number of players.
 */
unsigned char getNumPlayers() {
    unsigned char numPlayer = 0;
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
void getPlayerName(unsigned player, char* name) {
    int numChars = 0;
    char input;

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
    name[3] = '\0';
}

/**
 * Retrieves the desired number of worlds.
 *
 * <p>Number of worlds needs to be between 10 and 40.
 *
 * @return - Returns number of worlds.
 */
unsigned char getNumWorlds() {
    unsigned char numWorlds = 0;
    unsigned char numDigits = 0;
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
unsigned char getYears() {
    unsigned char numYears = 0;
    unsigned char numDigits = 0;
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
unsigned char getDefensive() {
    unsigned char answer = 2;
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
unsigned char getEvents() {
    unsigned char answer = 2;
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
