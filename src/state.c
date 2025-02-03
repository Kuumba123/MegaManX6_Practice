#include <common.h>
#include <gpu.h>
#include <object.h>
#include <layer.h>
#include "practice.h"

/*
 *   Being stored at the end of MOBJPAT.ARC/Player-Overlay (0x11D4 bytes free)
 */

#define VariableSetsCount 21
#define BuffersCount 8
#define BSS_ADDR 0x8000E910

#if BUILD == 1395
#define RNG *(uint16_t *)0x80090e70
#define RELOAD *(uint8_t *)0x800cc868
#define PASTBRIGHT *(uint16_t *)0x80097754
#define PASTBRIGHT2 *(uint16_t *)0x800c8858
#define PASTBRIGHT3 *(uint16_t *)0x800a21b6
#define UPDATECLUT *(uint8_t *)0x800c4560
#define STARTSELECT_FLAG *(uint32_t *)0x8008ec0c
#define SCREENBACKUP *(uint32_t *)0x800a21b0
#define VABP *(int *)0x800e4490
#define DECOMPRESS_ADDR 0x800c8868
#else
#define RNG *(uint16_t *)0x80092530
#define RELOAD *(uint8_t *)0x800cdf28
#define PASTBRIGHT *(uint16_t *)0x80098e14
#define PASTBRIGHT2 *(uint16_t *)0x800c9f18
#define PASTBRIGHT3 *(uint16_t *)0x800a3876
#define UPDATECLUT *(uint8_t *)0x800c5c20
#define STARTSELECT_FLAG *(uint32_t *)0x800902cc
#define SCREENBACKUP *(uint32_t *)0x800a3870
#define VABP *(int *)0x800e5ac0
#define DECOMPRESS_ADDR 0x800C9F28
#endif

#define FADE_F *(uint16_t *)0x801F8200
#define EXPO_F *(uint16_t *)0x801F8280

extern uint32_t swapTextureFlag;
extern void *swapTexturePointer;
extern void *clutPointer;
extern RECT clutRect;
extern Enemy *enemyDataPointers[];
extern uint8_t loadState; /*1=loading*/

#if BUILD == 1395
static void (*mode_A_Table[2])(Game *) = {0x8001ea28, 0x8001eb48};
#else
static void (*mode_A_Table[2])(Game *) = {0x8001fe78, 0x8001ff98};
#endif

static RECT blitRects[2] = {
    {0, 0, 256, 256},     // to buffer
    {320, 256, 256, 256} // to texture
};

void LoadCompressedImage(Object *objP, int16_t x, int16_t y);
void LoadBossRefightsArc();

extern void *freeAddress[];
extern uint16_t freeAddressSizes[];
extern void *readAddress[];
extern uint16_t addressesSize[];

extern void *maverickRefightBssAddresses[];
extern uint16_t maverickRefightBssSizes[];

extern void *stageBssAddresses[];
extern uint16_t *stageBssSizes[];

extern void *bonusBossAddresses[];
extern uint16_t bonusBossAddressesSize[];

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void SwapWeaponTexturesClut(Mega *megaP);

void LoadSigmaOverlay(int ovl);

void CheckPointCheck(Game *gameP);

void MemoryCopy(void *dest, const void *src, size_t size)
{
    // Ensure that the size is a multiple of 4 bytes
    size_t num_4byte_blocks = size / 4;

    uint32_t *d = (uint32_t *)dest;
    const uint32_t *s = (const uint32_t *)src;

    for (size_t i = 0; i < num_4byte_blocks; i++)
    {
        d[i] = s[i];
    }
}

uint32_t SwapTexture()
{
    if (buffer != 0)
    {
        return 1;
    }
    uint8_t* p = (uint8_t*)swapTexturePointer;

    for (size_t i = 0; i < 2; i++)
    {
        blitRects[1].x = 320 + i * 256;
        blitRects[1].y = 256;
        blitRects[1].h = 240;
        blitRects[0].h = 240;
        MoveImage(&blitRects[1], 0, 0);
        LoadImage(&blitRects[1], p);
        StoreImage(&blitRects[0], p);
        blitRects[1].y = 256 + 240;
        blitRects[1].h = 256 - 240;
        blitRects[0].h = 256 - 240;
        MoveImage(&blitRects[1], 0, 0);
        LoadImage(&blitRects[1], p + 0x1E000);
        StoreImage(&blitRects[0], p + 0x1E000);
        p += 0x20000;
    }
    practice.page ^= 1;
    return 2;
}
void SaveState()
{
    readAddress[0] = clutPointer;

    ThreadSleep(10); // Waiting before transfering

    // backup enemy data other vars
    Enemy *p = enemyDataPointers[game.stageId * 2 + game.mid];
    uint8_t *p2 = 0x1F800128;
    while ((p->type & 0xF) != 0xF)
    {
        *p2 = p->spawned;
        p += 1;
        p2 += 1;
    }

    int freeId = 0;
    int freeSize = freeAddressSizes[freeId];
    uint freeP = freeAddress[freeId];

    for (size_t i = 0; i < VariableSetsCount; i++)
    {
        int dumpSize = addressesSize[i];
        uint srcAddr = readAddress[i];

    SizeCheck:
        if (dumpSize > freeSize)
        {
            MemoryCopy(freeP, srcAddr, freeSize);
            srcAddr += freeSize;
            dumpSize -= freeSize;

            // Fix Free Data Vars
            freeId++;

            if (freeId > BuffersCount - 1)
            {
                printf("ERROR: went past MAX buffer counts: %X\n", BuffersCount);
                return;
            }

            freeSize = freeAddressSizes[freeId];
            freeP = freeAddress[freeId];
            goto SizeCheck;
        }
        else
        {
            MemoryCopy(freeP, srcAddr, dumpSize);
            freeP += dumpSize;
            freeSize -= dumpSize;
        }
    }

    practice.state.textureFlag = swapTextureFlag;
    practice.state.pastBright = PASTBRIGHT;
    practice.state.pastBright2 = PASTBRIGHT2;
    practice.state.pastBright3 = PASTBRIGHT3;
    practice.state.arcP = freeArcP;
    practice.state.reloadFlag = RELOAD;
    practice.state.page = practice.page;
    practice.state.rng = RNG;
    practice.state.sigmaOvl = practice.sigmaOvl;

    practice.state.made = true; // Mark State as Made

    if (game.point >= 2 && game.point <= 9 && game.point && game.stageId == 0xC && game.mid == 0)
    {
        MemoryCopy(BSS_ADDR, maverickRefightBssAddresses[game.point - 2], maverickRefightBssSizes[game.point - 2]);
    }
    else if (stageBssAddresses[game.stageId * 2 + game.mid] != 0)
    {
        MemoryCopy(BSS_ADDR, stageBssAddresses[game.stageId * 2 + game.mid], stageBssSizes[game.stageId * 2 + game.mid]);
    }
    else if (game.stageId > 0x12)
    {
        MemoryCopy(BSS_ADDR, bonusBossAddresses[*((uint8_t *)((int)&game + 1083))], bonusBossAddressesSize[*((uint8_t *)((int)&game + 1083))]);
    }
    UPDATECLUT = 1; // Update Clut
    MemoryCopy(SCREENBACKUP, *(uint32_t *)0x1F800008, practice.state.screenSize);
}
void LoadState()
{
    ThreadSleep(9); // Waiting before transfering

    // restore enemy data
    Enemy *p = enemyDataPointers[game.stageId * 2 + game.mid];
    uint8_t *p2 = 0x1F800128;
    while ((p->type & 0xF) != 0xF)
    {
        p->spawned = *p2;
        p->type = p->type & 0xF;
        p += 1;
        p2 += 1;
    }

    uint8_t pastPoint = game.point;
    uint8_t pastFlag = game.refights[4];
    uint8_t pastFile = RELOAD;

    int freeId = 0;
    int freeSize = freeAddressSizes[freeId];
    uint freeP = freeAddress[freeId];

    for (size_t i = 0; i < VariableSetsCount; i++)
    {
        int dumpSize = addressesSize[i];
        uint srcAddr = readAddress[i];

    SizeCheck:
        if (dumpSize > freeSize)
        {
            MemoryCopy(srcAddr, freeP, freeSize);
            srcAddr += freeSize;
            dumpSize -= freeSize;

            // Fix Free Data Vars
            freeId++;
            freeSize = freeAddressSizes[freeId];
            freeP = freeAddress[freeId];
            goto SizeCheck;
        }
        else
        {
            MemoryCopy(srcAddr, freeP, dumpSize);
            freeP += dumpSize;
            freeSize -= dumpSize;
        }
    }

    PASTBRIGHT = practice.state.pastBright;
    PASTBRIGHT2 = practice.state.pastBright2;
    PASTBRIGHT3 = practice.state.pastBright3;
    if (game.startingSong != 0)
    {
        EndSong();
    }

    freeArcP = practice.state.arcP;
    RELOAD = practice.state.reloadFlag;
    if (practice.keepRng)
    {
        RNG = practice.state.rng;
    }

    bgLayers[0].update = true;
    bgLayers[1].update = true;
    bgLayers[2].update = true;

    bool refightsBss = false;

    if (game.stageId == 0xC && game.mid == 0)
    {
        if (game.point >= 2 && game.point <= 9)
        {
            refightsBss = true;
        }

        if (refightsBss && game.point != pastPoint)
        {
            EndSong();
            LoadBossRefightsArc();
            ThreadSleep(2);
            FadeIn(10);
            while (fadeDirection != 0)
            {
                ThreadSleep(1);
            }
        }
        else
        {
            RELOAD = 0;
        }
    }
    else if (game.stageId == 0xC && game.mid == 1 && game.refights[4] == 1 && game.point == 1 && game.refights[4] != pastFlag)
    {
        EndSong();
        ArcSeek(0x85, 4, VABP);
        DrawLoad(0, 0);
        freeArcP = practice.state.arcP;
    }
    else if (stageBssAddresses[game.stageId * 2 + game.mid] != 0 && game.stageId < 0x13)
    {
        MemoryCopy(stageBssAddresses[game.stageId * 2 + game.mid], BSS_ADDR, stageBssSizes[game.stageId * 2 + game.mid]);
    }
    else if (game.stageId > 0x12)
    {
        MemoryCopy(bonusBossAddresses[*((uint8_t *)((int)&game + 1083))], BSS_ADDR, bonusBossAddressesSize[*((uint8_t *)((int)&game + 1083))]);
    }

    if (refightsBss)
    {
        MemoryCopy(maverickRefightBssAddresses[game.point - 2], BSS_ADDR, maverickRefightBssSizes[game.point - 2]);
    }
    if (practice.page != practice.state.page)
    {
        swapTextureFlag = 1;
    }
    else
    {
        swapTextureFlag = practice.state.textureFlag;
    }
    practice.sigmaOvl = practice.state.sigmaOvl;
    STARTSELECT_FLAG = 1;
    mega.newAnimeF = -1;
    LoadCompressedImage((Object *)&mega, 320, 0);
    if (mega.player == 0)
    {
        SwapWeaponTexturesClut(&mega);
    }
    UPDATECLUT = 1; // Update Clut
    MemoryCopy(*(uint32_t *)0x1F800008, SCREENBACKUP, practice.state.screenSize);
    ThreadSleep(1);
}

void StateCheck(Game *gameP)
{
    int time = gameP->stageTime;
    int totalSeconds = time / 60;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    int frames = time % 60;

    DrawDebugText(5, 3, 0, "%2d:%2d:%2d", minutes, seconds, frames);

    if (loadState == 1)
    {
        DrawDebugText(5, 4, 2, "(LOADING)");
    }

    if (loadState != 1 && FADE_F == 0 && EXPO_F == 0)
    {
        if ((buttonsPressed & (PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1 | PAD_SELECT | PAD_LEFT | PAD_RIGHT | PAD_TRIANGLE)) != 0)
        {
            if ((buttonsHeld & (PAD_R2 + PAD_SELECT)) == (PAD_R2 + PAD_SELECT))
            {
                SaveState();
            }
            else if ((buttonsHeld & (PAD_L2 + PAD_SELECT)) == (PAD_L2 + PAD_SELECT) && practice.state.made)
            {
                LoadState();
            }
            else
            {
                CheckPointCheck(gameP);
            }
        }
    }
    mode_A_Table[gameP->mode2](gameP);
}

#undef RNG
#undef RELOAD
#undef PASTBRIGHT
#undef STARTSELECT_FLAG
#undef SCREENBACKUP
#undef VABP
#undef UPDATECLUT
#undef FADE_F
#undef EXPO_F

#undef DECOMPRESS_ADDR