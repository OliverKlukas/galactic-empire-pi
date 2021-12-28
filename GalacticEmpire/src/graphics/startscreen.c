#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include "graphics.h"
#include "startscreen.h"


/*****************************************************************************/
/*                              Functions                                    */
/*****************************************************************************/


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
