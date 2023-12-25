#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
#include "graphics.h"
#include "../main.h"
#include "box.h"


/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/


/**
 *  Deletes current text in input field.
 */
void clearTextIOField() {
    cclearxy(textLine1X, textLine1Y, MAP_N_LINES_VERTICAL - 2);
    cclearxy(textLine2X, textLine2Y, MAP_N_LINES_VERTICAL - 2);
}

/**
 * Converts planet letter 'a', 'b' etc to index in galaxy struct 0 1 2.
 *
 * @param planetLetter Char planet letter, e.g. 'a', 'b'
 * @return Returns index in galaxy struct 0 1 2.
 */
int convertPlanetLetterToNumberIndex(char planetLetter) {
    if (planetLetter > 85) {
        return planetLetter - 173;
    }
    return planetLetter - 65;
}

/**
 * Handles input of single planet letters.
 *
 *   1. What it should be: enter one character, press enter : returns int which is the index of the entered world in the galaxy struct
 *   2. If more or less than one character, return -1: display wrong input in parent function
 *   3. If space is entered: return -2: to restart the entire input
 *   4. If only enter is pressed: -3 is returned
 *
 * @return Returns index or failure code.
 */
int readSinglePlanetLetter() {
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

/**
 *  Reads number inputs.
 *
 *   1. What it should be: reads any kind of number entered, press enter : returns fitting int number
 *   2. if any non digit is entered: return -1
 *   3. If nothing or 0 is entered: return -1 // 0 is handled by parent function
 *   4. If a number with more than 5 digits is entered: return -1
 *   5. If space is entered: return -2: cancel the entire loop
 *
 * @return Returns fitting int number.
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
 * Retrieves inputs of game rounds.
 *
 * @param playerIter - Current active player, unsigned 0 - (numPlayer-1)
 * @param playerName
 * @param galaxy
 * @param numWorlds
 * @return - Returns integer array with [(player, origin, destination, ships), ...] that contains the inputs. returns [-1,-1,-1,-1] if the player is done putting in inputs and it is the next one's turn.
 */
int *retrieveInputs(int playerIter, char* playerName, world **galaxy, int numWorlds) {
    static int inputs [4];

    char readChar;
    int origin = -1;
    int destination = -1;
    int nShips = -1;

    while (1) {
        // 1st question: Origin
        while (origin < 0) {
            textcolor(textColor);
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
                sleep(2);
                clearTextIOField();
            } else if (origin == -2) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Cancelled!");
                sleep(2);
                clearTextIOField();
            } else if (origin == -3) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "End turn?");
                readChar = cgetc();
                if (readChar == CH_ENTER) {
                    origin = -1;
                    break;
                }
            } else if (!(galaxy[origin]->owner == playerIter)) {
                clearTextIOField();
                cputsxy(textLine1X, textLine1Y, "Wrong input!");
                cputsxy(textLine2X, textLine2Y, "Not your world!");
                origin = -1;
                sleep(2);
                clearTextIOField();
            }
        }

        // 2nd question: Destination
        while (destination < 0 && origin >= 0) {
            clearTextIOField();
            textcolor(textColor);
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

        // 3rd Question: # ships:
        while (nShips < 0 && origin >= 0) {
            clearTextIOField();
            textcolor(textColor);
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
            } else if (nShips > galaxy[origin]->ships) {
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
        break;
    }

    inputs[0] = playerIter;
    inputs[1] = origin;
    inputs[2] = destination;
    inputs[3] = nShips;
    return inputs;
}

/**
 * Displays reinforcements arriving at planets.
 *
 * @param playerIter
 * @param worldIter
 * @param nShips
 */
void displayReinforcements(unsigned playerIter, unsigned worldIter, unsigned nShips)
{
    clearTextIOField();
    cputsxy(textLine1X, textLine1Y, "World");
    if (worldIter < 20) {
        placeColoredLetter(textLine1X + 6, textLine1Y, worldIter + 65, playerIter);
    } else {
        placeColoredLetter(textLine1X + 6, textLine1Y, worldIter + 173, playerIter);
    }
    cputsxy(textLine1X + 7, textLine1Y, ": ");
    gotoxy(textLine1X + 9, textLine1Y);
    cprintf("%hi", nShips);
    cputsxy(textLine2X, textLine2Y, "ships arrived!");
    sleep(3);
}

/**
 * Simulates attack on a planet and updates the galaxy.
 *
 * @param allNames - All player names 1-x and Me at 0.
 * @param world - World that's under attack.
 * @param attacker - Integer value of attacker player.
 * @param numShips - Number of ships the attacker is attacking with.
 */
void simulateFight(world **galaxy, char **allNames, unsigned world, unsigned attacker, unsigned numShips){
    // Attack variables.
    unsigned prevColor;
    unsigned fightValue;
    unsigned deciderValue = 49;
    unsigned shipsLostPerLoss;
    int attackerShips = numShips;

    // Attack message.
    cputsxy(textLine1X, textLine1Y, "Attack on world: ");
    if(world < 20){
        placeColoredLetter(textLine1X + 16, textLine1Y, world + 65, galaxy[world]->owner);
    } else{
        placeColoredLetter(textLine1X + 16, textLine1Y, world + 173, galaxy[world]->owner);
    }
    sleep(3);

    // Defender and attacker.
    clearTextIOField();
    gotoxy(textLine1X, textLine1Y);
    prevColor = textcolor(playerColors[attacker]);
    cprintf("Attacker: %s", allNames[attacker]);
    textcolor(playerColors[galaxy[world]->owner]);
    gotoxy(textLine2X, textLine2Y);
    cprintf("Defender: %s", allNames[galaxy[world]->owner]);
    textcolor(prevColor);
    sleep(3);

    // Print initial battle state.
    clearTextIOField();
    gotoxy(textLine1X, textLine1Y);
    textcolor(playerColors[attacker]);
    cprintf("Attacker: %d", attackerShips);
    gotoxy(textLine2X, textLine2Y);
    textcolor(playerColors[galaxy[world]->owner]);
    cprintf("Defender: %d", galaxy[world]->ships);
    sleep(1);

    // Simulate attack and give out state of battle.
    while(attackerShips > 0 && galaxy[world]->ships > 0){
        // Set shipsLostPerLoss dynamically with the number of ships left.
        if(attackerShips < galaxy[world]->ships && attackerShips > 31){
            shipsLostPerLoss = attackerShips / 8;
        } else if(attackerShips >= galaxy[world]->ships && galaxy[world]->ships > 31){
            shipsLostPerLoss = galaxy[world]->ships / 8;
        } else {
            shipsLostPerLoss = 3;
        }

        // Calculate who loses one round.
        fightValue = rand() % 100;
        if(fightValue < deciderValue){
            // Attacker loses ships.
            attackerShips -= shipsLostPerLoss;
            if(attackerShips < 0){
                break;
            }
        } else{
            // Defender loses ships.
            if(shipsLostPerLoss >= galaxy[world]->ships){
                break;
            }
            galaxy[world]->ships -= shipsLostPerLoss;
        }

        // Slowly change decider value so that attacker loses more.
        if(deciderValue < 74){
            deciderValue += 1;
        }

        // Update current battle state.
        clearTextIOField();
        gotoxy(textLine1X, textLine1Y);
        textcolor(playerColors[attacker]);
        cprintf("Attacker: %d", attackerShips);
        gotoxy(textLine2X, textLine2Y);
        textcolor(playerColors[galaxy[world]->owner]);
        cprintf("Defender: %d", galaxy[world]->ships);
        sleep(1);
    }

    // Check if attacker won.
    if(attackerShips > 0){
        galaxy[world]->owner = attacker;
        galaxy[world]->ships = attackerShips;
    }

    // End message.
    textcolor(prevColor);
    clearTextIOField();
    gotoxy(textLine1X, textLine1Y);
    cprintf("%s won!", allNames[galaxy[world]->owner]);
    sleep(3);
}

/**
 * Display supernova information, special events.
 *
 * @param world - Destination where the ships should have gone to.
 * @param numShips - Number of ships lost.
 * @param player - Player who lost the ships as string.
 * @param allNames - All player names 1-x and Me at 0.
 */
void supernova(unsigned world, unsigned numShips, unsigned player, char *allNames){
    clearTextIOField();
    cputsxy(textLine1X, textLine1Y, "SUPERNOVA world: ");
    if(world < 20){
        placeColoredLetter(textLine1X + 16, textLine1Y, world + 65, player);
    } else{
        placeColoredLetter(textLine1X + 16, textLine1Y, world + 173, player);
    }
    gotoxy(textLine2X, textLine2Y);
    textcolor(playerColors[player]);
    cputs(&allNames[player*4]);
    textcolor(textColor);
    cprintf(" lost %d ships", numShips);
    cgetc();
    clearTextIOField();
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

    // Erase text IO field.
    clearTextIOField();

    return answer;
}
