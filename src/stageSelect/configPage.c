#include <common.h>
#include <gpu.h>
#include <layer.h>
#include "../practice.h"

#if BUILD == 1395
#define RECT_X 0
#define RECT_Y 0
#define RECT_W 512
#define RECT_H 240
#else
#define RECT_X 16
#define RECT_Y 16
#define RECT_W 480
#define RECT_H 208
#endif

static TILE rect[2];

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void CreateTitleScreenThread();

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
    else if ((buttonsPressed & PAD_SELECT) != 0)
    {
        RECT rect = {0x340,0x100,0x40,0x100};
        LoadImage(&rect,freeArcP);
        DrawSync(0);
        CreateTitleScreenThread();
        DeleteThread();
    }
    
    
    DrawDebugText(25,2,2,"CONFIG PAGE");
}

#undef RECT_X
#undef RECT_Y
#undef RECT_W
#undef RECT_H