#include <stdio.h>
#include <conio.h>
#include <stdbool.h>

// Global key variable
int numPlayers = 1;
int numWorlds = 10;
char *playerNames;

bool defensiveShips = true;
int events = true;

int **worlds;

// todo: delete
int key;
char x,y=10;

// functions
void drawMap()
{}

void drawTable()
{}

void updateInput()
{}



int main()
{
    
    
    /* Clear Screen */
    clrscr();
 
    /* Hide cursor */
    cursor(0);
 
    drawMap();
    drawTable();



    /* Loop until Q is pressed */
    while ((key  = cgetc()) != 'Q')
    {
        
        /*
        // Delete the character
        cputsxy(x,y,".");
 
        // keys;
        switch (key) 
        { 
            case 'w': 
                y--; 
                break; 
            case 'a': 
                x--; 
                break; 
            case 's': 
                y++; 
                break; 
            case 'd': 
                x++; 
                break; 
 
            default:
                break;
        }
 
        cputsxy(x,y,"@");
        */
    }
    
    
    cursor(1);
    
    return(0);
}

