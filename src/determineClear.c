#include <common.h>
#include <object.h>
#include <misc.h>
#include "practice.h"

#if BUILD == 1395
#define RELOAD *(uint8_t *)0x800cc868
#else
#define RELOAD *(uint8_t *)0x800cdf28
#endif

extern bool LevelMidTable[];

extern uint32_t swapTextureFlag;
extern int8_t checkPointNew;
extern uint8_t exitType;
extern uint8_t exitStage;
extern void * endFreeArcP;

static int8_t checkPointTextureFlags[] =
    {
        6, 0,    // ST00
        0xE, 0,    // ST01
        0x3C, 0,    // ST02
        0xF8, 0,    // ST03
        2, 6,    // ST04
        6, 0,    // ST05
        0x38, 0,    // ST06
        0xE, 0,    // ST07
        2, 0,    // ST08
        0, 0,    // ST09
        0, 0,    // ST0A
        0, 0,    // ST0B
        0, 0,    // ST0C
        0, 0,    // ST0D
        0, 0,    // ST0E
        0, 0,    // ST0F
        0x1E, 0,    // ST10
        0xC, 0,    // ST11
        0x1E, 0,    // ST12
        0, 0,    // ST13
        0, 0,    // ST14
        0, 0,    // ST15
        0, 0     // ST16
};

void SwapTexture(bool sync);
void LoadScreens();

void DetermineClear(Game *gameP)
{
    exitType = gameP->clear < 1;
    if (gameP->clear == 0)
    {
        /*MegaMan Dead*/
        gameP->spawnFlags = 0;
        bool reset = false;

        if (gameP->stageId == 0xC && gameP->mid == 0 && gameP->point > 1)
        {
            reset = gameP->point < 10;
        }

        /* Teleporter Room in Refights */
        if (reset)
        {
            gameP->point = 1;
        }

        EndSong(); // PS2 POPS Fix

        LoadScreens();

        gameP->mode = 9;
    }
    else
    {
        if (gameP->clear < 0)
        {
            gameP->spawnFlags = 0xFF;
            gameP->hpTemp = mega.hp;
            gameP->weaponTemp = mega.weapon;

            memcpy(&gameP->ammoTemp[0], &mega.ammo[0], 32);

            gameP->mode = 9;

            if ((uint8_t)gameP->clear == 0xC1)
            {
                freeArcP = endFreeArcP;
                gameP->point = checkPointNew;
                gameP->weaponTemp = 0;
                gameP->hpTemp = gameP->maxHPs[gameP->player];
                for (size_t i = 0; i < 16; i++)
                {
                    gameP->ammoTemp[i] = gameP->maxAmmos[gameP->player] * 6;
                }
                if (gameP->stageId != 0xC || gameP->mid != 0)
                {
                    if (practice.page != ((checkPointTextureFlags[gameP->stageId * 2 + gameP->mid] & (1 << gameP->point)) != 0))
                    {
                        swapTextureFlag = 1;
                    }
                }
            }
        }
        else // Actual Real Clear
        {
            gameP->spawnFlags = 0;

            if (gameP->stageId == 0) // Intro
            {
                gameP->exitType = 0xFF;
                gameP->mode = 0xB;
            }
            else if (gameP->stageId < 9) // 8 Maverick stages
            {
                if (gameP->clear == 0x41) // GOTO Alternative Half
                {
                    // Set new Stage Id
                    gameP->mid = (gameP->stageId + -1) & 1;
                    gameP->stageId = ((gameP->stageId + -1) / 2) + 0x13;

                    gameP->mode = 8;
                }
                else if (LevelMidTable[gameP->stageId] == false && gameP->mid == 0)
                {
                    gameP->mid = 1;
                    gameP->mode = 8;
                }
                else
                {
                    gameP->exitType = 0xFF;
                    gameP->mode = 0xB;
                }
            }
            else if (gameP->stageId == 0xC && gameP->mid == 0)
            {
                gameP->mid = 1;
                gameP->mode = 8;
            }
            else if (gameP->stageId == 0x11)
            {
                gameP->stageId = 0x12;
                gameP->mode = 8;
            }
            else
            {
                gameP->exitType = 0xFF;
                gameP->mode = 0xB;
            }
        }
    }
    gameP->clear = 0;
    gameP->mode2 = 0;
    gameP->mode3 = 0;
    gameP->mode4 = 0;
}
void ResetState()
{
    practice.state.made = false;
    practice.page = 0;
    practice.sigmaOvl = 0;
    LoadLevel();
}
#undef RELOAD