#include <common.h>
#include <gpu.h>
#include <layer.h>
#include "../practice.h"

#define Cursor gameP->mode4
#if BUILD == 1395
#define RECT_X 0
#define RECT_Y 0
#define RECT_W 512
#define RECT_H 240
#define VOLUME_ADDR 0x80018128
#else
#define RECT_X 16
#define RECT_Y 16
#define RECT_W 480
#define RECT_H 208
#define VOLUME_ADDR 0x80019560
#endif

static TILE rect[2];

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void CreateTitleScreenThread();

static char* CategoryStrings[9] = {"ALL STAGES","ALL STAGES (NEW ROUTE)","ANY%","100%","ALL STAGES UN-ARMORED","ANY% ULTIMATE ARMOR","ANY% BLACK ZERO","MINIMALIST X-TREME","CUSTOM"};
static char* DifficultyStrings[] = {"EASY","NORMAL","X-TREME"};

void ConfigPage(Game * gameP) //Mode 4-6
{
    setTile(&rect[buffer]);
    setRGB0(&rect[buffer],0,0,0);
    setXY0(&rect[buffer], RECT_X, RECT_Y);
    setWH(&rect[buffer], RECT_W, RECT_H);
    addPrim(&drawP->ot[0],&rect[buffer]);
    
    if ((buttonsPressed & PAD_R1) != 0) //Go Back to Stage Page
    {
        gameP->mode2 = 4;
        gameP->mode3 = 0;
        gameP->mode4 = 0;
        PlaySound(5,0,0);
        return;
    }
    else if ((buttonsPressed & PAD_SELECT) != 0) //Go to Title Screen
    {
        RECT rect = {0x340,0x100,0x40,0x100};
        LoadImage(&rect,freeArcP);
        DrawSync(0);
        CreateTitleScreenThread();
        DeleteThread();
    }

    bool toggle = (buttonsPressed & CONFIRM) != 0;

    if ((buttonsPressed & PAD_DOWN) != 0)
    {
        if (Cursor == 3)
        {
            Cursor = 0;
        }
        else
        {
            Cursor += 1;
        }
    }
    else if ((buttonsPressed & PAD_UP) != 0)
    {
        if (Cursor == 0)
        {
            Cursor = 3;
        }
        else
        {
            Cursor -= 1;
        }
    }
    
    if (Cursor == 0) //Category
    {
        if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            if (practice.category == 8)
            {
                practice.category = 0;
            }
            else
            {
                practice.category += 1;
            }
        }
        else if ((buttonsPressed & PAD_LEFT) != 0)
        {
            if (practice.category == 0)
            {
                practice.category = 8;
            }
            else
            {
                practice.category -= 1;
            }
        }
        
    }
    else if (Cursor == 1 && toggle)
    {
        practice.keepRng ^= 1;
    }
    else if (Cursor == 2 && toggle)
    {
        practice.cancelXA ^= 1;
    }
    else
    {
        if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            if (game.difficulty == 2)
            {
                game.difficulty = 0;
            }
            else
            {
                game.difficulty += 1;
            }
        }
        else if ((buttonsPressed & PAD_LEFT) != 0)
        {
            if (game.difficulty == 0)
            {
                game.difficulty = 2;
            }
            else
            {
                game.difficulty -= 1;
            }
        }
        
    }
    if (practice.cancelXA)
    {
        *(int8_t*)VOLUME_ADDR = 0;
    }
    else
    {
        *(int8_t*)VOLUME_ADDR = 0x73;
    }
    DrawDebugText(17,4 + Cursor,1,">");
    DrawDebugText(18,4,0,"CATEGORY\nKEEP RNG\nCANCEL XA\nDIFFICULTY");
    DrawDebugText(33,4,0,"%s\n%d\n%d\n%s",CategoryStrings[practice.category],practice.keepRng,practice.cancelXA,DifficultyStrings[game.difficulty]);
    DrawDebugText(25,2,2,"CONFIG PAGE");
    DrawDebugText(18, 22, 1, "PRESS SELECT TO\nRETURN TO TITLE SCREEN");
}

#undef RECT_X
#undef RECT_Y
#undef RECT_W
#undef RECT_H
#undef Cursor