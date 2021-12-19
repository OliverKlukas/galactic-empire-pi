#include <stdio.h>
#include <stdlib.h>
#include <cc65.h>
#include <conio.h>
#include <stdbool.h>
#include <tgi.h>
// #include <math.h>

#ifndef DYN_DRV
    #  define DYN_DRV       0
#endif

/* Driver stuff */
static unsigned MaxX;
static unsigned MaxY;
static unsigned AspectRatio;



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

// todo could be made variable later
int mapXLength = 20;
int mapYLength = 20;

int i;
int linesHalfThickness;

static void CheckError (const char* S)
{
    unsigned char Error = tgi_geterror ();

    if (Error != TGI_ERR_OK) {
        printf ("%s: %u\n", S, Error);
        if (doesclrscrafterexit ()) {
            cgetc ();
       }
        exit (EXIT_FAILURE);
    }
}

 

#if DYN_DRV
static void DoWarning (void)
/* Warn the user that the dynamic TGI driver is needed for this program */
{
    printf ("Warning: This program needs the TGI\n"
            "driver on disk! Press 'y' if you have\n"
            "it - any other key exits.\n");
    if (tolower (cgetc ()) != 'y') {
        exit (EXIT_SUCCESS);
    }
    printf ("OK. Please wait patiently...\n");
}
#endif



void drawThickVerticalLine(const int startX, const int startY, const int length, int thickness)
{
    // Only works for straight lines ! 
    // thickness has to be an uneven number

    if (thickness > 1 && thickness % 2)
    {
        thickness += 1;
    }
    
    linesHalfThickness = (int) thickness/2;

    for (i = -linesHalfThickness; i <=linesHalfThickness; ++i)
    {
        tgi_line(startX + i, startY, startX + i, startY + length);
    }
}


// functions
void drawMap()
{
    tgi_clear ();
    tgi_line(2,2,2,10);
    tgi_line(3,2,3,10);

    drawThickVerticalLine(10,10,10,1);
    drawThickVerticalLine(15,10,10,2);
    
    tgi_bar (20, 20, 30, 30);


    chlinexy(5, 5, 10); 

    /*
    chlinexy(0, 0, 10);
    cputsxy(10, 0, "A");
    chlinexy(12, 0, 10);
    chlinexy(0, 1, 10);


    for (i = 0; i < mapXLength; ++i)
    {
        cvlinexy(i, 0, mapYLength);    
    }
    chlinexy(0, 0, 1);

    cputsxy(30, 5, "+++");
    */

    //for (i = 0; i < mapYLength; ++i)
    //{
    //    chlinexy(0, i, mapXLength);    
    // }
    cgetc();
}

void drawTable()
{}

void updateInput()
{}



int main()
{
#if DYN_DRV
    /* Warn the user that the tgi driver is needed */
    DoWarning ();

    /* Load and initialize the driver */
   tgi_load_driver (tgi_stddrv);
    CheckError ("tgi_load_driver");
#else
    /* Install the driver */
    tgi_install (tgi_static_stddrv);
    CheckError ("tgi_install");
#endif

    tgi_init ();
    CheckError ("tgi_init");

    /* Get stuff from the driver */
    MaxX = tgi_getmaxx ();
    MaxY = tgi_getmaxy ();
    AspectRatio = tgi_getaspectratio ();


    /// todo: Graphics stuff goes here 
    /* Clear Screen */
    clrscr();
 
    /* Hide cursor */
    cursor(0);
 
    drawMap();
    drawTable();


#if DYN_DRV
    /* Unload the driver */
    tgi_unload ();
#else
    /* Uninstall the driver */
    tgi_uninstall ();
#endif

    /* Reset the border */
    // todo do later
    // (void) bordercolor (Border);
 
    /* Done */

    while(1)
    {}

    printf ("Done\n");
    return EXIT_SUCCESS;
}
