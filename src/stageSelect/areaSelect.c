#include <common.h>
#include <object.h>
#include <gpu.h>
#include <misc.h>
#include "../practice.h"
#define Cursor gameP->var[0]
#define AreaMode gameP->refights[2]
#define Timer *(uint16_t *)((int)gameP + 8)
#define CategoryOptionData *(uint8_t *)((int)gameP + 4)

static enum Ranks {
    UH,
    PA,
    GA,
    SA,
    A,
    B,
    C,
    D
};

/*All Stages (Old Route)%*/
static uint8_t allStagesOldMavericksClearedTable[2][8] = {{0xC0, 0xED, 0xE9, 0xE1, 0xEF, 0, 0, 0x40}, {0xE0, 0xED, 0xE9, 0xE1, 0xEF, 0, 0, 0x40}};
static uint8_t allStagesOldMavericksPlayerTable[2][8] = {{1, 1, 0, 1, 0, 0, 1, 1}, {1, 1, 0, 1, 0, 0, 1, 1}};
static uint32_t allStagesOldMavericksPartsTable[2][8] = {{0, 0x400010, 0, 0x400010, 0, 0, 0, 0}, {0x400010, 0x400010, 0x400010, 0x400010, 0, 0, 0, 0}};
static uint8_t allStagesOldMavericksNightmareTable[2][8] = {{8, 3, 4, 0, 4, 0, 0, 0}, {6, 3, 4, 0, 4, 0, 0, 0}};
static uint8_t allStagesOldMavericksRankTable[2][8] = {{A, SA, D, SA, D, D, D, C}, {A, SA, D, SA, D, D, D, C}};

/*All Stages (New Route)%*/
static uint8_t allStagesMavericksClearedTable[2][8] = {{0xC0, 0xE1, 0xFB, 0xE3, 0xEB, 0, 0, 0x40}, {0xE0, 0xE1, 0xEB, 0xE3, 0xEF, 0, 0, 0x40}};
static uint8_t allStagesMavericksPlayerTable[2][8] = {{1, 1, 1, 1, 0, 0, 1, 1}, {1, 1, 0, 1, 0, 0, 1, 1}};
static uint32_t allStagesMavericksPartsTable[2][8] = {{0, 0x400010, 0x400010, 0x400010, 0, 0, 0, 0}, {0x400010, 0x400010, 0, 0x400010, 0, 0, 0, 0}};
static uint8_t allStagesMavericksNightmareTable[2][8] = {{8, 7, 4, 2, 4, 0, 0, 0}, {6, 7, 4, 2, 4, 0, 0, 0}};
static uint8_t allStagesMavericksRankTable[2][8] = {{B, SA, GA, SA, D, D, D, C}, {A, SA, D, SA, D, D, D, C}};

/*100%*/
static uint8_t hundoMavericksClearedTable[2][8] = {{0xC0, 0xF1, 0xFB, 0xF3, 0xE1, 0, 0, 0x40}, {0, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF}}; 
static uint8_t hundoMavericksPlayerTable[2][8] = {{1, 1, 1, 1, 1, 0, 1, 1,},{0, 1, 0, 0, 0, 1, 0, 1,}};
static uint8_t hundoMavericksArmorTypeTable[2][8] = {{5, 5, 5, 5, 5, 1, 5, 5}, {0, 5, 3, 1, 3, 5, 1, 5}};
static uint8_t hundMavericksArmorPartsTable[2][8] = {{0x40, 0x49, 0x49, 0x49, 0x48, 0, 0x40, 0x40,}, {0, 0x7F, 0x5F, 0x4D, 0x5F, 0x48, 0x49, 0x5D}};

/*All Stages Un-Armored X*/
static uint8_t allStagesUnArmoredMavericksClearedTable[8] = {0x44, 4, 0, 0x47, 0x4F, 0, 4, 0x5F};
static uint8_t allStagesUnArmoredMavericksBonusBossTable[8] = {1, 0, 0, 1, 1, 0, 1, 1};
static uint32_t allStagesUnArmoredMavericksPartsTable[8] = {0x80018, 0, 0, 0x80010, 0x80010, 0, 0, 0x80010};
static uint8_t allStagesUnArmoredMavericksNightmareTable[8] = {6, 3, 0, 2, 4, 0, 0, 0};
static uint8_t allStagesUnArmoredRankTable[8] = {A, B, D, SA, SA, D, B, GA};
static bool allStagesExTankFlagTable[8] = {true, false, false, true, true, false, true, true};

/*Minimalist X-Treme*/
static uint8_t minimalistMavericksCleartedTable[8] = {0xB2, 0x90, 0xFB, 0xF3, 0, 0x92, 0xB3, 0x10};
static uint8_t minimalistMavericksNightmareTable[8] = {6, 0, 0, 2, 0, 8, 1, 0};
static uint8_t minimalistRankTable[8] = {SA, A, GA, GA, D, SA, GA, B};

static uint8_t categoryMaverickOptionTable[9][8] = {
    {0x00, 0x00, 0x00, 0x40, 0x00, 0x83, 0x00, 0x00},  // All Stages (Old Route)
    {0x00, 0x00, 0x00, 0x40, 0x00, 0x83, 0x00, 0x00},  // All Stages (New Route)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F},  // Any%
    {0x00, 0x8C, 0x8C, 0x8D, 0x8C, 0x83, 0x8C, 0x8C},  // 100%
    {0x00, 0x83, 0x00, 0x40, 0x00, 0x83, 0x00, 0x00},  // All Stages Un-Armored
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F},  // Any% Ultimate
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F},  // Any% Zero
    {0x00, 0x8C, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00},  // Min X-Treme
    {0xC0, 0xC0, 0xC0, 0x50, 0xC0, 0xC0, 0xC0, 0xC0}}; // Custom

/*
 *   bbxm rrvv
 *
 * b = Beginning Area Mode (0-3)
 * m = Goto nightmare Area Mode Option after Start Option is selected
 * r = Area Mode to goto after Revist Option is selected
 * v = Area Mode to goto after Vist Option is selected
 */

/////////////////////////
static int8_t sigmaStageTable[4] = {0x10, 0x11, 0xC, 0x0};

static bool isRevist = false;
static bool isNightmare = false;

static int8_t sigmaStage = -1;

void CalculateNightmareLevel(int8_t stageId, int8_t *stageIdP, int8_t *midP);
void SaveRestore();

void AreaSelectInit(Game *gameP)
{
    /*
     *   0 = Sigma 1,2,3, & Intro
     *   1 = Start & Mid
     *   2 = Vist & ReVist
     *   3 = Normal & Nightmare
     */

    Cursor = 0;
    AreaMode = 0;
    Timer = 0;
    isRevist = false;
    isNightmare = false;
    gameP->refights[3] = 0;
    gameP->refights[4] = 0;
    gameP->mid = 0;

    if (gameP->stageId != 0xC)
    {
        CategoryOptionData = categoryMaverickOptionTable[practice.category][gameP->stageId - 1];
        AreaMode = CategoryOptionData >> 6;
        if (AreaMode == 0)
        {
            gameP->mode3 = 7;
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
                uint8_t option = 0;
                if (AreaMode == 2) // Vist & ReVist
                {
                    isRevist = Cursor;
                    if (isRevist == false)
                    {
                        option = CategoryOptionData & 3;
                    }
                    else
                    {
                        option = (CategoryOptionData >> 2) & 3;
                    }
                    if (option != 0)
                    {
                        AreaMode = option;
                        Cursor = 0;
                        return;
                    }
                }
                else if (AreaMode == 1) // Start & Mid
                {
                    gameP->mid = Cursor;
                    if ((CategoryOptionData & 16) != 0 && Cursor == 0)
                    {
                        AreaMode = 3;
                        Cursor = 0;
                        return;
                    }
                }
                else // Normal & Nightmare
                {
                    isNightmare = Cursor;
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
                        else if (sigmaStage == 2)
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
        gameP->ranks[0] = D;
        gameP->ranks[1] = D;
        gameP->bonusBoss = 1;
        gameP->igt = 0;
        gameP->stageTime = 0;
        gameP->playerSouls[0] = 0;
        gameP->playerSouls[1] = 0;
        gameP->souls = 0;

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
            gameP->refights[1] = 1; // Skip Text (Gate)
        }
        for (size_t i = 0; i < 128; i++)
        {
            SetReploidStatus(i, 0);
        }

        if (practice.category == CUSTOM)
        {
            practice.orginStage = gameP->stageId;
            if (isNightmare && gameP->stageId != 0 && gameP->stageId < 9)
            {
                CalculateNightmareLevel(gameP->stageId, &gameP->stageId, &gameP->mid);
            }

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
            if (practice.category == ANY_PERCENT_ULTIMATE)
            {
                gameP->armorType = 4;
            }
            else
            {
                gameP->armorType = 1;
            }
        }
        else
        {
            uint8_t difficulty = gameP->difficulty;
            if (difficulty == 2)
            {
                difficulty = 1;
            }

            if (practice.category == ALL_STAGES)
            {
                if (gameP->stageId > 8)
                {
                    gameP->clearedStages = 0xFF;
                    parts = 0x400010;
                    gameP->ranks[1] = GA;
                }
                else
                {
                    int8_t i = gameP->stageId - 1;
                    gameP->clearedStages = allStagesMavericksClearedTable[difficulty][i];
                    gameP->player = allStagesMavericksPlayerTable[difficulty][i];
                    parts = allStagesMavericksPartsTable[difficulty][i];
                    gameP->nightmareEffects[gameP->stageId] = allStagesMavericksNightmareTable[difficulty][i];
                    gameP->nightmareEffectId = allStagesMavericksNightmareTable[difficulty][i];
                    gameP->ranks[gameP->player] = allStagesMavericksRankTable[difficulty][i];

                    if (isRevist == false && gameP->stageId == 6) // Fighting Zero instead of High-Max
                    {
                        gameP->bonusBoss = 0;
                    }

                    if (isRevist) // for Rainy Turtloid
                    {
                        SetSeenText(2);
                        SetReploidStatus(0x58,2);
                        gameP->ranks[1] = A;
                        gameP->player = 1;
                        gameP->nightmareEffects[6] = 8;
                        gameP->nightmareEffectId = 8;
                        parts = 0x400010;
                        if (difficulty == 0)
                        {
                            gameP->clearedStages = 0xC1;
                        }
                        else
                        {
                            gameP->clearedStages = 0xC0;
                        }
                    }

                    if (gameP->player == 0)
                    {
                        gameP->armorType = 1; // Falcon Armor
                    }
                }
            }
            else if (practice.category == ALL_STAGES_OLD)
            {
                if (gameP->stageId > 8)
                {
                    gameP->clearedStages = 0xFF;
                    parts = 0x400010;
                    gameP->ranks[1] = GA;
                }
                else
                {
                    int8_t i = gameP->stageId - 1;
                    gameP->clearedStages = allStagesOldMavericksClearedTable[difficulty][i];
                    gameP->player = allStagesOldMavericksPlayerTable[difficulty][i];
                    parts = allStagesOldMavericksPartsTable[difficulty][i];
                    gameP->nightmareEffects[gameP->stageId] = allStagesOldMavericksNightmareTable[difficulty][i];
                    gameP->nightmareEffectId = allStagesOldMavericksNightmareTable[difficulty][i];
                    gameP->ranks[gameP->player] = allStagesOldMavericksRankTable[difficulty][i];

                    if (isRevist == false && gameP->stageId == 6) // Fighting Zero instead of High-Max
                    {
                        gameP->bonusBoss = 0;
                    }

                    if (isRevist) // for Rainy Turtloid
                    {
                        SetSeenText(2);
                        SetReploidStatus(0x58,2);
                        gameP->ranks[1] = A;
                        gameP->player = 1;
                        gameP->nightmareEffects[6] = 8;
                        gameP->nightmareEffectId = 8;
                        parts = 0x400010;
                        if (difficulty == 0)
                        {
                            gameP->clearedStages = 0xC1;
                        }
                        else
                        {
                            gameP->clearedStages = 0xC0;
                        }
                    }

                    if (gameP->player == 0)
                    {
                        gameP->armorType = 1; // Falcon Armor
                    }
                }
                if ((gameP->clearedStages & 1) != 0) // give player sub tank + life up
                {
                    gameP->tanks |= 0x1000;
                    gameP->tanksAmmo[0] = 12;
                    parts |= 0x8000;
                }
                if ((gameP->clearedStages & 8) != 0) // give zero heart tank from metal shark
                {
                    gameP->maxHPs[1] = 34;
                }
            }
            else if (practice.category == ANY_PERCENT || practice.category == ANY_PERCENT_ZERO)
            {
                if (practice.category == ANY_PERCENT_ZERO)
                {
                    gameP->armors |= 0x20;
                }
                
                if (gameP->stageId > 8)
                {
                    gameP->clearedStages = 0x40;
                    gameP->bonusBoss = 2;
                }
                else
                {
                    if (gameP->stageId == 8)
                    {
                        gameP->player = 0;
                        gameP->armorType = 1; // Falcon Armor
                        if (isRevist)
                        {
                            SetSeenText(1);
                            SetReploidStatus(0x7F,2);
                            gameP->clearedStages = 0x40;
                        }
                        else
                        {
                            gameP->bonusBoss = 0;
                        }
                    }
                }
            }
            else if (practice.category == HUNDO)
            {
                if (gameP->stageId > 8)
                {
                    gameP->clearedStages = 0xFF;
                    gameP->bonusBoss = 2;
                    if (gameP->stageId >= 0x10 || gameP->stageId <= 0x12)
                    {
                        gameP->player = 0;
                        gameP->armorType = 3;
                    }
                    else
                    {
                    }
                }
                else
                {
                    int8_t i = gameP->stageId - 1;
                    gameP->clearedStages = hundoMavericksClearedTable[isRevist][i];
                    gameP->player = hundoMavericksPlayerTable[isRevist][i];
                    gameP->armorType = hundoMavericksArmorTypeTable[isRevist][i];
                    gameP->armorParts = hundMavericksArmorPartsTable[isRevist][i];

                    if ((gameP->armorParts & 0xF) == 0xF)
                    {
                        gameP->armors |= 2;
                    }
                    if ((gameP->armorParts & 0xF0) == 0xF0)
                    {
                        gameP->armors |= 4;
                    }
                    
                    if (isRevist)
                    {
                        if (gameP->stageId == 6)
                        {
                            SetSeenText(2);
                        }
                        else if (gameP->stageId == 8)
                        {
                            SetSeenText(1);
                        }
                    }
                    else if (gameP->stageId == 6)
                    {
                        gameP->bonusBoss = 0;
                    }

                    if (gameP->clearedStages == 0xFF)
                    {
                        gameP->bonusBoss = 2;
                    }
                }
            }
            else if (practice.category == ALL_STAGES_UNARMORED)
            {
                gameP->player = 0;
                gameP->armorType = 0;

                if (gameP->stageId > 8)
                {
                    parts = 0x88018;
                    gameP->clearedStages = 0xFF;
                    gameP->ranks[0] = GA;
                }
                else
                {
                    int8_t i = gameP->stageId - 1;
                    gameP->clearedStages = allStagesUnArmoredMavericksClearedTable[i];
                    gameP->bonusBoss = allStagesUnArmoredMavericksBonusBossTable[i];
                    parts = allStagesUnArmoredMavericksPartsTable[i];
                    gameP->nightmareEffects[gameP->stageId] = allStagesUnArmoredMavericksNightmareTable[i];
                    gameP->nightmareEffectId = allStagesUnArmoredMavericksNightmareTable[i];
                    gameP->ranks[0] = allStagesUnArmoredRankTable[i];
                    if (allStagesExTankFlagTable[i] == true)
                    {
                        gameP->tanks = 0x8000;
                    }

                    if (isRevist)
                    {
                        gameP->bonusBoss = 1;
                        if (gameP->stageId == 6)
                        {
                            SetSeenText(2);
                            SetReploidStatus(0x58,2);
                            gameP->clearedStages = 0xDF;
                            gameP->tanks = 0x8000;
                            parts = 0x88018;
                            gameP->ranks[0] = GA;
                            gameP->nightmareEffects[6] = 8;
                            gameP->nightmareEffectId = 8;
                        }
                        else
                        {
                            SetReploidStatus(0x1C,2);
                            SetReploidStatus(0x1E,2);
                            SetReploidStatus(0x15,2);
                            gameP->clearedStages = 0x45;
                            gameP->tanks = 0x8000;
                            parts = 0x80018;
                            gameP->ranks[0] = A;
                            gameP->nightmareEffects[2] = 7;
                            gameP->nightmareEffectId = 7;
                        }
                    }
                }
                if ((gameP->clearedStages & 1) != 0) // give player sub tank
                {
                    gameP->tanks |= 0x1000;
                    gameP->tanksAmmo[0] = 12;
                }
            }
            else if (practice.category == ANY_PERCENT_ULTIMATE)
            {
                gameP->player = 0;
                gameP->armorType = 4;

                if (gameP->stageId > 8)
                {
                    gameP->clearedStages = 2;
                }
                else
                {
                    if (gameP->stageId == 8)
                    {
                        gameP->clearedStages = 2;
                        if (!isRevist)
                        {
                            gameP->bonusBoss = 0;
                        }
                        else
                        {
                            SetSeenText(1);
                            SetReploidStatus(0x7F,2);
                        }
                    }
                }
            }
            else // MIN X-Treme
            {
                gameP->player = 0;
                gameP->armorType = 0;

                if (gameP->stageId > 8)
                {
                    gameP->clearedStages = 0xFF;
                    gameP->ranks[0] = GA;
                    gameP->tanks = 0x8000;
                    if (gameP->stageId == 0x10)
                    {
                        parts = 8;
                    }
                }
                else
                {
                    int8_t i = gameP->stageId - 1;
                    gameP->clearedStages = minimalistMavericksCleartedTable[i];
                    gameP->ranks[0] = minimalistRankTable[i];
                    gameP->nightmareEffects[gameP->stageId] = minimalistMavericksNightmareTable[i];
                    gameP->nightmareEffectId = minimalistMavericksNightmareTable[i];

                    if (isRevist)
                    {
                        gameP->clearedStages = 0xFF;
                        gameP->ranks[0] = GA;
                        gameP->nightmareEffects[2] = 3;
                        gameP->nightmareEffectId = 3;
                    }
                }
            }
        }
        if (isNightmare)
        {
            CalculateNightmareLevel(gameP->stageId, &gameP->stageId, &gameP->mid);
        }
        
        gameP->equipedParts[gameP->player] = parts;
        /**************/

        gameP->mode = 7;
        gameP->mode2 = 0;
        gameP->mode3 = 0;
        gameP->mode4 = 0;
        SaveRestore();
    }
}

#undef Cursor
#undef AreaMode
#undef Timer
#undef CategoryOptionData