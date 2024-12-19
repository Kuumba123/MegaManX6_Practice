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
extern uint8_t exitType;
extern uint8_t exitStage;

void SwapTexture(bool sync);

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

        LoadLevel();

        RELOAD = 0; // for non refight boss textures

        if (practice.page != 0)
        {
            SwapTexture(false);
            practice.page = 1;
        }
        practice.state.made = false;

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