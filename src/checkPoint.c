#include <common.h>
#include <gpu.h>
#include "practice.h"

#if BUILD == 1395
#define MENU_TXT "X = SPAWN\nO = RESTART\n^ = EXIT"
#define RESET PAD_CIRCLE
#define RELOAD *(uint8_t *)0x800cc868
#define MAIN_THREAD 0x8001e6e4
#else
#define MENU_TXT "O  = SPAWN\n[] = RESTART\nX  = EXIT"
#define RESET PAD_SQUARE
#define RELOAD *(uint8_t *)0x800cdf28
#define MAIN_THREAD 0x8001fb34
#endif

static struct Restore
{
    uint32_t reploids[16];
    uint32_t hearts;
    uint16_t tanks;
    int8_t maxHP;
    int8_t maxAmmo;
    uint8_t armors;
    uint8_t armorParts;
    int8_t stageId;
    int8_t mid;
    int8_t tanksAmmo[3];
    bool seen;
};
static struct Restore restore;

int8_t checkPointNew = 0xFF;

int8_t maxCheckPoint[] =
    {
        2, 0,  // ST00
        3, 0,  // ST01
        5, 0,  // ST02
        7, 0,  // ST03
        1, 2,  // ST04
        2, 0,  // ST05
        5, 0,  // ST06
        3, 0,  // ST07
        1, 0,  // ST08
        0, 0,  // ST09
        0, 0,  // ST0A
        0, 0,  // ST0B
        17, 1, // ST0C
        0, 0,  // ST0D
        0, 0,  // ST0E
        0, 0,  // ST0F
        4, 0,  // ST10
        3, 0,  // ST11
        4, 0,  // ST12
        1, 1,  // ST13
        1, 1,  // ST14
        1, 1,  // ST15
        1, 1   // ST16
};

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void SaveRestore()
{
    restore.hearts = game.hearts;
    restore.tanks = game.tanks;
    restore.maxHP = game.maxHPs[game.player];
    restore.maxAmmo = game.maxAmmos[game.player];
    restore.armors = game.armors;
    restore.armorParts = game.armorParts;
    restore.stageId = game.stageId;
    restore.mid = game.mid;
    restore.seen = game.seenTextBoxes[0];
    restore.tanksAmmo[0] = game.tanksAmmo[0];
    restore.tanksAmmo[1] = game.tanksAmmo[1];
    restore.tanksAmmo[2] = game.tanksAmmo[2];
    memcpy(&restore.reploids, &game.reploids, 16 * 4);
}
void LoadRestore()
{
    game.hearts = restore.hearts;
    game.tanks = restore.tanks;
    game.maxHPs[game.player] = restore.maxHP;
    game.maxAmmos[game.player] = restore.maxAmmo;
    game.armors = restore.armors;
    game.armorParts = restore.armorParts;
    game.stageId = restore.stageId;
    game.mid = restore.mid;
    game.igt = 0;
    game.stageTime = 0;
    *(int8_t*)((int)&game + 1124) = 0;
    
    uint16_t val = 0;
    if (restore.seen)
    {
        val = 0xFFFF;
    }
    for (size_t i = 0; i < 20; i++)
    {
        game.seenTextBoxes[i] = val;
    }
    game.tanksAmmo[0] = restore.tanksAmmo[0];
    game.tanksAmmo[1] = restore.tanksAmmo[1];
    game.tanksAmmo[2] = restore.tanksAmmo[2];
    memcpy(&game.reploids, &restore.reploids, 16 * 4);
}

void CheckPointCheck(Game *gameP)
{
    if (gameP->clear == 0 && mega.hp != 0)
    {
        if ((buttonsHeld & (PAD_RIGHT + PAD_SELECT)) == (PAD_RIGHT + PAD_SELECT))
        {
            checkPointNew = gameP->point + 1;
            gameP->clear = 0xC1;
        }
        else if ((buttonsHeld & (PAD_LEFT + PAD_SELECT)) == (PAD_LEFT + PAD_SELECT))
        {
            checkPointNew = gameP->point - 1;
            gameP->clear = 0xC1;
        }
        else if ((buttonsHeld & (PAD_L1 + PAD_R1 + PAD_TRIANGLE + PAD_SELECT)) == (PAD_L1 + PAD_R1 + PAD_TRIANGLE + PAD_SELECT))
        {
            // Checkpoint Menu
            int mouse = 0;
            int8_t max = maxCheckPoint[game.stageId * 2 + game.mid];

            while (true) // Loop
            {
                if ((buttonsPressed & CANCEL) != 0)
                {
                    break;
                }

                // Check Inputs
                if ((buttonsPressed & CONFIRM) != 0)
                {
                    checkPointNew = mouse;
                    game.clear = 0xC1;
                    break;
                }
                else if ((buttonsPressed & RESET) != 0)
                {
                    game.mode = 8;
                    game.mode2 = 0;
                    game.mode3 = 0;
                    game.mode4 = 0;
                    game.point = 0;
                    game.mid = 0;
                    RELOAD = 0;
                    TurnOffSound(0xFF,0);
                    EndSong();
                    LoadRestore();
                    NewThread2(MAIN_THREAD);
                }

                if ((buttonsPressed & PAD_DOWN) != 0)
                {
                    mouse++;
                }
                else if ((buttonsPressed & PAD_UP) != 0)
                {
                    mouse--;
                }

                if (mouse < 0)
                {
                    mouse = max;
                }
                else if (mouse > max)
                {
                    mouse = 0;
                }

                DrawDebugText(3, 5 + mouse, 1, ">");
                for (size_t i = 0; i < (max + 1); i++)
                {
                    DrawDebugText(4, 5 + i, 0, "POINT %d", i);
                }
                DrawDebugText(18, 22, 1, MENU_TXT);
                DrawDebugText(12, 3, 2, "CHECKPOINT PAGE");
                ThreadSleep(1);
            }
        }

        if (checkPointNew > maxCheckPoint[gameP->stageId * 2 + gameP->mid])
        {
            checkPointNew = 0;
        }
        else if (checkPointNew < 0)
        {
            checkPointNew = maxCheckPoint[gameP->stageId * 2 + gameP->mid];
        }
    }
}

#undef MENU_TXT
#undef RESET
#undef RELOAD
#undef MAIN_THREAD