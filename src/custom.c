#include <common.h>
#include <misc.h>
#include "practice.h"

#define PAGE_TOTAL 5
#define Cursor gameP->mode3

static int page;

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);
void SaveRestore();

static char *rankText[] = {"UH", "PA", "GA", "SA", "A", "B", "C", "D"};

void CustomRoute(Game *gameP)
{
    if (gameP->mode2 == 0)
    {
        gameP->mode2 = 1;
        FadeIn(7);
        return;
    }
    else if (gameP->mode2 == 1)
    {
        if (fadeDirection != 0)
        {
            return;
        }
        gameP->mode2 = 2;
        gameP->mode3 = 0;
        gameP->mode4 = 0;
    }

    int parts = 0;

    while (true)
    {
        if ((buttonsPressed & PAD_START) != 0)
        {
            if (gameP->player != 0) // Zero
            {
                gameP->armorType = 5;
            }
            else if (gameP->player == 0 && gameP->armorType > 4)
            {
                gameP->armorType = 0;
            }
            if ((gameP->armorParts & 0xF) == 0xF || gameP->armorType == 2) // Blade Armor
            {
                gameP->armorParts |= 0xF;
                gameP->armors |= 2;
            }
            if ((gameP->armorParts & 0xF0) == 0xF0 || gameP->armorType == 3) // Shadow Armor
            {
                gameP->armorParts |= 0xF0;
                gameP->armors |= 4;
            }
            if (gameP->armorType == 4) // Ultimate Armor
            {
                gameP->armors |= 8;
            }
            if (gameP->armorType == 1) // Falcon Armor
            {
                gameP->armors |= 1;
            }
            if (((gameP->tanks & 0x1000)) != 0) // Sub-Tank 1
            {
                gameP->tanksAmmo[0] |= 0x80;
            }
            if (((gameP->tanks & 0x2000)) != 0) // Sub-Tank 2
            {
                gameP->tanksAmmo[1] |= 0x80;
            }
            if (gameP->player == 1 || gameP->bonusBoss > 0) // Unlocked Zero
            {
                gameP->armors |= 0x10;
            }

            gameP->equipedParts[gameP->player] |= parts;
            gameP->parts = gameP->equipedParts[gameP->player];

            gameP->stageId = gameP->slowMotion;
            gameP->mid = gameP->maverickShow;

            gameP->mode = 7;
            gameP->mode2 = 0;
            gameP->mode3 = 0;
            gameP->mode4 = 0;
            SaveRestore();
            break;
        }

        bool toggle = (buttonsPressed & CONFIRM) != 0;

        switch (page)
        {
        case 0: // WEAPON PAGE
            if (toggle)
            {
                gameP->clearedStages ^= (1 << Cursor);
            }
            for (size_t i = 0; i < 8; i++)
            {
                DrawDebugText(21, 5 + i, 0, "%d", (gameP->clearedStages & (1 << i)) != 0);
            }

            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 7)
                {
                    Cursor += 1;
                }
                else
                {
                    Cursor = 0;
                }
            }
            else if ((buttonsPressed & PAD_UP) != 0)
            {
                if (Cursor != 0)
                {
                    Cursor -= 1;
                }
                else
                {
                    Cursor = 7;
                }
            }
            DrawDebugText(12, 3, 2, "WEAPON PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "YAMMAR OPTION\nICE BURST\nMAGMA BLADE\nMETAL ANCHOR\nGROUND DASH\nMETEOR RAIN\nGUARD SHELL\nRAY ARROW");
            break;

        case 1: // ARMOR PAGE
            /*
             * HELMET,ARM,BODY,LEG
             */

            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 11)
                {
                    Cursor += 1;
                }
                else
                {
                    Cursor = 0;
                }
            }
            else if ((buttonsPressed & PAD_UP) != 0)
            {
                if (Cursor != 0)
                {
                    Cursor -= 1;
                }
                else
                {
                    Cursor = 11;
                }
            }

            if (Cursor < 8)
            {
                if (toggle)
                {
                    gameP->armorParts ^= (1 << Cursor);
                }
            }
            else if (Cursor == 8)
            {
                if (toggle)
                {
                    gameP->armors ^= 8; // Ultimate Armor
                }
            }
            else if (Cursor == 9)
            {
                if (toggle)
                {
                    gameP->armors ^= 0x20; // Black Zero
                }
            }
            else if (Cursor == 10)
            {
                if (toggle)
                {
                    gameP->player ^= 1;
                }
            }
            else
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    if (gameP->armorType != 5)
                    {
                        gameP->armorType += 1;
                    }
                    else
                    {
                        gameP->armorType = 0;
                    }
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    if (gameP->armorType != 0)
                    {
                        gameP->armorType -= 1;
                    }
                    else
                    {
                        gameP->armorType = 5;
                    }
                }
            }

            for (size_t i = 0; i < 8; i++)
            {
                DrawDebugText(21, 5 + i, 0, "%d", (gameP->armorParts & (1 << i)) != 0);
            }
            DrawDebugText(21, 13, 0, "%d\n%d\n%d\n%d", (gameP->armors & 8) != 0, (gameP->armors & 0x20) != 0, gameP->player, gameP->armorType);

            DrawDebugText(12, 3, 2, "ARMOR PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "BLADE-HELMET\nBLADE-ARM\nBLADE-BODY\nBLADE-LEG\nSHADOW-HELMET\nSHADOW-ARM\nSHADOW-BODY\nSHADOW-LEG\nULTIMATE-ARMOR\nBLACK-ZERO\nPLAYER\nARMOR-TYPE");
            break;

        case 2: // TANK PAGE
            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 9)
                {
                    Cursor += 1;
                }
                else
                {
                    Cursor = 0;
                }
            }
            else if ((buttonsPressed & PAD_UP) != 0)
            {
                if (Cursor != 0)
                {
                    Cursor -= 1;
                }
                else
                {
                    Cursor = 9;
                }
            }

            if (Cursor == 0)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->mode4 += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->mode4 -= 1;
                }
                gameP->mode4 &= 7;

                if (toggle)
                {
                    gameP->hearts ^= (1 << gameP->mode4);
                }
            }
            else if (Cursor < 5)
            {
                if (toggle)
                {
                    gameP->tanks ^= (0x1000 << (Cursor - 1));
                }
            }
            else if (Cursor < 8)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->tanksAmmo[Cursor - 5] += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->tanksAmmo[Cursor - 5] -= 1;
                }
            }

            else if (Cursor == 8)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->maxHPs[gameP->player] += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->maxHPs[gameP->player] -= 1;
                }
            }
            else
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->maxAmmos[gameP->player] += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->maxAmmos[gameP->player] -= 1;
                }
            }

            DrawDebugText(21, 5, 0, "%d", (gameP->hearts & (1 << gameP->mode4)) != 0);

            for (size_t i = 0; i < 4; i++)
            {
                DrawDebugText(21, 6 + i, 0, "%d", (gameP->tanks & (0x1000 << i)) != 0);
                if (i != 3)
                {
                    DrawDebugText(21, 10 + i, 0, "%d", gameP->tanksAmmo[i]);
                }
            }
            DrawDebugText(21, 13, 0, "%d\n%d", gameP->maxHPs[gameP->player], gameP->maxAmmos[gameP->player]);

            DrawDebugText(12, 3, 2, "TANK PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "HEART-%d:\nSUB-TANK 1\nSUB-TANK 2\nWEAPON-TANK\nEX-TANK\nTANK AMMO 1\nTANK AMMO 2\nTANK AMMO 3\nMAX HP\nMAX AMMO", gameP->mode4 + 1);
            break;

        case 3: // PARTS PAGE

            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 19)
                {
                    Cursor += 1;
                }
                else
                {
                    Cursor = 0;
                }
            }
            else if ((buttonsPressed & PAD_UP) != 0)
            {
                if (Cursor != 0)
                {
                    Cursor -= 1;
                }
                else
                {
                    Cursor = 19;
                }
            }

            if (toggle)
            {
                if (Cursor < 16)
                {
                    parts ^= 4 << Cursor;
                }
                else if (gameP->player == 0)
                {
                    gameP->equipedParts[0] ^= 4 << Cursor;
                }
                else
                {
                    gameP->equipedParts[1] ^= 0x40 << Cursor;
                }
            }

            DrawDebugText(12, 3, 2, "PARTS PAGE");

            if (Cursor < 16) // 1st 16 Parts
            {
                DrawDebugText(3, 5 + Cursor, 1, ">");
                for (size_t i = 0; i < 16; i++)
                {
                    DrawDebugText(21, 5 + i, 0, "%d", (parts & (4 << i)) != 0);
                }
                DrawDebugText(4, 5, 0, "SPEEDSTER\nJUMPER\nHYPER DASH\nENERGY SAVER\nSUPER RECOVER\nBUSTER PLUS\nSPEED SHOT\nSHOCK BUFFER\nD-BARRIER\nD-CONVERTER\nHYPER DRIVE\nPOWER DRIVE\nWEAPON DRIVE\nLIFE RECOVER\nW.RECOVER\nOVER DRIVE");
            }
            else // Player specfic parts
            {
                int val;

                if (gameP->player == 0)
                {
                    val = 0x40000;
                    DrawDebugText(4, 5, 0, "RAPID S\nULTIMATE BUSTER\nQUICK CHARGE\nWEAPON PLUS");
                }
                else
                {
                    val = 0x400000;
                    DrawDebugText(4, 5, 0, "SABER PLUS\nSABER EXTEND\nSHOT ERASER\nMASTER SABER");
                }
                for (size_t i = 0; i < 4; i++)
                {
                    DrawDebugText(21, 5 + i, 0, "%d", (gameP->equipedParts[gameP->player] & (val << i)) != 0);
                }
                DrawDebugText(3, 5 + Cursor - 16, 1, ">");
            }
            DrawDebugText(28, 22, 2, "%d/%d", (Cursor > 15) + 1, 2);
            break;

        case 4: // OTHER PAGE
            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 5)
                {
                    Cursor += 1;
                }
                else
                {
                    Cursor = 0;
                }
            }
            else if ((buttonsPressed & PAD_UP) != 0)
            {
                if (Cursor != 0)
                {
                    Cursor -= 1;
                }
                else
                {
                    Cursor = 5;
                }
            }

            if (Cursor == 0 && practice.orginStage > 0 && practice.orginStage < 9)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->nightmareEffects[practice.orginStage] += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->nightmareEffects[practice.orginStage] -= 1;
                }
            }
            else if (Cursor == 1)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->stageSelectMode += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->stageSelectMode -= 1;
                }
            }
            else if (Cursor == 2)
            {
                if (toggle)
                {
                    for (size_t i = 0; i < 20; i++)
                    {
                        game.seenTextBoxes[i] ^= 0xFF;
                    }
                }
            }
            else if (Cursor == 3)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0 && gameP->ranks[gameP->player] != 7)
                {
                    gameP->ranks[gameP->player] += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0 && gameP->ranks[gameP->player] != 0)
                {
                    gameP->ranks[gameP->player] -= 1;
                }
            }
            else if(Cursor == 4)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0 && gameP->bonusBoss != 2)
                {
                    gameP->bonusBoss += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0 && gameP->bonusBoss != 0)
                {
                    gameP->bonusBoss -= 1;
                }
            }
            else if(practice.orginStage < 9 && practice.orginStage != 0)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->mode4 += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->mode4 -= 1;
                }
                gameP->mode4 &= 0xF;
                
                if (toggle)
                {
                    if (GetReploidStatus((practice.orginStage - 1) * 16 + gameP->mode4) == 0)
                    {
                        SetReploidStatus((practice.orginStage - 1) * 16 + gameP->mode4, 2);
                    }
                    else
                    {
                        SetReploidStatus((practice.orginStage - 1) * 16 + gameP->mode4, 0);
                    }
                }
            }

            DrawDebugText(12, 3, 2, "OTHER PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "NIGHTMARE SOURCE\nSELECT MODE\nSEEN BOXES\nPLAYER RANK\nBONUS BOSS\nREPLOID-%d", gameP->mode4 + 1);
            DrawDebugText(21, 5, 0, "%d\n%d\n%d\n%s\n%d\n%d", gameP->nightmareEffects[practice.orginStage], gameP->stageSelectMode, gameP->seenTextBoxes[0] != 0, rankText[gameP->ranks[gameP->player]], gameP->bonusBoss, GetReploidStatus((practice.orginStage - 1) * 16 + gameP->mode4) != 0);
            break;

        default:
            break;
        }
        int8_t past = page;

        if ((buttonsPressed & PAD_R1) != 0)
        {
            if (page != (PAGE_TOTAL - 1))
            {
                page += 1;
            }
            else
            {
                page = 0;
            }
        }
        else if ((buttonsPressed & PAD_L1) != 0)
        {
            if (page != 0)
            {
                page -= 1;
            }
            else
            {
                page = (PAGE_TOTAL - 1);
            }
        }

        if (past != page)
        {
            Cursor = 0;
            gameP->mode4 = 0;
        }

        DrawDebugText(28, 3, 2, "%d/%d", page + 1, PAGE_TOTAL);
        DrawDebugText(9, 24, 1, "PRESS START TO\nSTART THE STAGE");
        ThreadSleep(1);
    }
}

#undef PAGE_TOTAL
#undef Cursor