#include <common.h>
#include <object.h>
#include <gpu.h>
#include "../practice.h"
#define Cursor gameP->var[0]
#define AreaMode gameP->refights[2]
#define Timer *(uint16_t *)((int)gameP + 8)

void AreaSelectInit(Game * gameP)
{
    Cursor = 0;
    AreaMode = 0;
    gameP->refights[3] = 0;
    gameP->refights[4] = 0;

    Object * p = GetMiscObject();
    p->flags = 1;
    p->id = 0x2B;
    p->stageVar = 0;

    p = GetMiscObject();
    p->flags = 1;
    p->id = 0x2B;
    p->stageVar = 1;

    p = GetMiscObject();
    p->flags = 1;
    p->id = 0x2B;
    p->stageVar = 2;

    if (gameP->stageId != 0xC)
    {
        AreaMode = 1;
    }

    gameP->mode3 = 5;
}
void AreaSelect(Game * gameP)
{
    int pastCursor = Cursor;

    if ((buttonsPressed & (PAD_LEFT + PAD_RIGHT)) != 0)
    {
        if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            Cursor += 1;
        }
        else
        {
            Cursor -= 1;
        }
        if (AreaMode != 0)
        {
            Cursor &= 1;
        }
        else
        {
            if (Cursor < 0)
            {
                Cursor = 2;
            }
            Cursor %= 3;
        }
    }
    if (pastCursor == Cursor)
    {
        if ((buttonsPressed & (PAD_START + CONFIRM)) == 0)
        {
            if ((buttonsPressed & CANCEL) != 0) //Triangle was pressed , go back to maverick select
            {
                gameP->mode3 = 9;
                gameP->refights[4] = 1;
                Timer = 0;
            }
        }
        else
        {
            /*Determine Area Select Option (Sigma 1,2,3 or Start,Mid or Vist,ReVist or Normal,Nightmare)*/
            //TODO: add route checks and such here
            gameP->mode3 = 7;
            Timer = 0;
            PlaySound(5,1,0);
        }
    }
    else
    {
        Timer = 0;
        gameP->mode3 = 6;
        PlaySound(5,0,0);
    }
}
void AreaSelectCoolDown(Game * gameP)
{
    Timer = Timer + 1;
    if (Timer > 7)
    {
        Timer = 0;
        gameP->mode3 = 5;
    }
}
void AreaSelected(Game * gameP)
{
    Timer = Timer + 1;
    if (Timer > 7)
    {
        Timer = 0;
        gameP->refights[3] = 0;
        gameP->mode3 = 8;
        FadeOut(8);
    }
}
void AreaDetermine(Game* gameP)
{
    if (fadeDirection == 0)
    {
        RECT rect = {0x340,0x100,0x40,0x100};
        LoadImage(&rect,freeArcP);
        DrawSync(0);
        ClearAll();

        //Pre-Clear Nightmare Effect
        for (size_t i = 0; i < 16; i++)
        {
            gameP->nightmareEffects[i] = 0;
        }





        /**************/
        for (size_t i = 0; i < 8; i++)
        {
            gameP->refights[i] = 0;
        }
        gameP->mode = 7;
        gameP->mode2 = 0;
        gameP->mode3 = 0;
        gameP->mode4 = 0;
    }
}

#undef Cursor
#undef AreaMode
#undef Timer