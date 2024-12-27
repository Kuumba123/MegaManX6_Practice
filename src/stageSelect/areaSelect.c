#include <common.h>
#include <object.h>
#include <gpu.h>
#include "../practice.h"
#define Cursor gameP->var[0]
#define AreaMode gameP->refights[2]
#define Timer *(uint16_t *)((int)gameP + 8)

/*All Stages (New Route)%*/
static uint8_t mavericksClearedTable[8] = {0, 0, 0, 0, 0, 0, 0, 0x40};

void AreaSelectInit(Game *gameP)
{
    Cursor = 0;
    AreaMode = 0;
    gameP->refights[3] = 0;
    gameP->refights[4] = 0;

    Object *p = GetMiscObject();
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
void AreaSelect(Game *gameP)
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
            if ((buttonsPressed & CANCEL) != 0) // Triangle was pressed , go back to maverick select
            {
                gameP->mode3 = 9;
                gameP->refights[4] = 1;
                Timer = 0;
            }
        }
        else
        {
            /*Determine Area Select Option (Sigma 1,2,3 or Start,Mid or Vist,ReVist or Normal,Nightmare)*/
            // TODO: add route checks and such here
            gameP->mode3 = 7;
            Timer = 0;
            PlaySound(5, 1, 0);
        }
    }
    else
    {
        Timer = 0;
        gameP->mode3 = 6;
        PlaySound(5, 0, 0);
    }
}
void AreaSelectCoolDown(Game *gameP)
{
    Timer = Timer + 1;
    if (Timer > 7)
    {
        Timer = 0;
        gameP->mode3 = 5;
    }
}
void AreaSelected(Game *gameP)
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
void AreaDetermine(Game *gameP)
{
    if (fadeDirection == 0)
    {
        RECT rect = {0x340, 0x100, 0x40, 0x100};
        LoadImage(&rect, freeArcP);
        DrawSync(0);
        ClearAll();

        uint32_t parts = 0;
        gameP->equipedParts[0] = 0;
        gameP->equipedParts[1] = 0;
        gameP->armorParts = 0;
        gameP->armors = 0;
        gameP->clearedStages = 0;
        gameP->tanks = 0;
        gameP->hearts = 0;
        gameP->maxHPs[0] = 32;
        gameP->maxHPs[1] = 32;
        gameP->maxAmmos[0] = 48;
        gameP->maxAmmos[1] = 48;
        gameP->tanksAmmo[0] = 0;
        gameP->tanksAmmo[1] = 0;
        gameP->tanksAmmo[2] = 0;
        gameP->ranks[0] = 5;
        gameP->ranks[1] = 3;
        gameP->bonusBoss = 1;

        // Pre-Clear Nightmare Effect
        for (size_t i = 0; i < 16; i++)
        {
            gameP->nightmareEffects[i] = 0;
        }
        // Pre-Clear Seen Text Boxes
        for (size_t i = 0; i < 20; i++)
        {
            gameP->seenTextBoxes[i] = 0;
        }

        gameP->player = 1; // Default to Zero
        gameP->armorType = 5;

        //Clear Re-Fights
        for (size_t i = 0; i < 8; i++)
        {
            gameP->refights[i] = 0;
        }
        if (gameP->stageId != 0xC || gameP->mid != 0)
        {
            gameP->refights[0] = 1; // Skip Text
        }

        if (practice.category == CUSTOM)
        {
            gameP->mode = 5;
            gameP->mode2 = 0;
            gameP->mode3 = 0;
            gameP->mode4 = 0;
            return;
        }

        if (gameP->stageId == 0) // Intro
        {
            gameP->stageSelectMode = 0;
        }
        else
        {
            if (practice.category == ALL_STAGES)
            {
                if (gameP->stageId <= 8)
                {
                    gameP->clearedStages = mavericksClearedTable[gameP->stageId - 1];
                }
            }
        }

        /**************/

        gameP->mode = 7;
        gameP->mode2 = 0;
        gameP->mode3 = 0;
        gameP->mode4 = 0;
    }
}

#undef Cursor
#undef AreaMode
#undef Timer