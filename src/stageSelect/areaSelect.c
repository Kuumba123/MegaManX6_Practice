#include <common.h>
#include <object.h>
#include <gpu.h>
#include "../practice.h"
#define Cursor gameP->var[0]
#define AreaMode gameP->refights[2]
#define Timer *(uint16_t *)((int)gameP + 8)

/*All Stages (New Route)%*/
static uint8_t mavericksClearedTable[8] = {0, 0, 0, 0, 0, 0, 0, 0x40};

/////////////////////////
static int8_t sigmaStageTable[4] = {0x10, 0x11, 0xC, 0x0};

static bool isRevist = false;
static bool isNightmare = false;

static int8_t sigmaStage = -1;

void AreaSelectInit(Game *gameP)
{
    Cursor = 0;
    AreaMode = 0;
    isRevist = false;
    isNightmare = false;
    gameP->refights[3] = 0;
    gameP->refights[4] = 0;
    gameP->mid = 0;

    /*
     *   0 = Sigma 1,2,3, & Intro
     *   1 = Start & Mid
     *   2 = Vist & ReVist
     *   3 = Normal & Nightmare
     */

    if (gameP->stageId != 0xC)
    {
        AreaMode = 1;

        if (gameP->stageId == 6 && (practice.category == ALL_STAGES || practice.category == ALL_STAGES_OLD))
        {
            AreaMode = 2;
        }
        else if (practice.category == ALL_STAGES || practice.category == ALL_STAGES_OLD)
        {
            gameP->mode3 = 6;
            return;
        }
    }

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
                Cursor = 3;
            }
            Cursor %= 4;
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
            // route checks and such here
            PlaySound(5, 1, 0);
            if (gameP->stageId == 0xC && AreaMode == 0)
            {
                sigmaStage = Cursor;
            }

            if (gameP->stageId <= 8)
            {
                if (gameP->stageId == 6 && (practice.category == ALL_STAGES || practice.category == ALL_STAGES_OLD))
                {
                    if (AreaMode == 2) // Vist/ReVist
                    {
                        isRevist = Cursor;
                    }

                    if (isRevist == false)
                    {
                        AreaMode = 3;
                        Cursor = 0;
                        return;
                    }
                    else
                    {
                        isNightmare = Cursor;
                    }
                }
            }
            else
            {
                if (sigmaStage == 3)
                {
                    gameP->stageId = 0;
                }
                else
                {
                    if (AreaMode == 0)
                    {
                        if (sigmaStage == 1 || sigmaStage == 2)
                        {
                            AreaMode = 1;
                            Cursor = 0;
                            return;
                        }
                        gameP->stageId = sigmaStageTable[sigmaStage];
                    }
                    else
                    {
                        if (sigmaStage == 1 && Cursor != 0)
                        {
                            gameP->stageId = 0x12;
                        }
                        else if(sigmaStage == 2)
                        {
                            gameP->mid = Cursor;
                        }
                        else
                        {
                            gameP->stageId = sigmaStageTable[sigmaStage];
                        }
                    }
                }
            }

            gameP->mode3 = 7;
            Timer = 0;
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
        gameP->armors = 0x11;
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

        // Clear Re-Fights
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
            gameP->player = 0;
            gameP->armorType = 1;
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