#include <common.h>
#include <gpu.h>
#include <object.h>
#include <layer.h>
#include "practice.h"

/*
 *   Being stored at the end of MOBJPAT.ARC/Player-Overlay (0x11D4 bytes free)
 */

#define VariableSetsCount 21
#define BuffersCount 7
#define BSS_ADDR 0x801E3E4C

#if BUILD == 1395
#define RNG *(uint16_t *)0x80090e70
#define RELOAD *(uint8_t *)0x800cc868
#define PASTBRIGHT *(uint8_t *)0x800a21b6
#define UPDATECLUT *(uint8_t *)0x800c4560
#define STARTSELECT_FLAG *(uint32_t *)0x8008ec0c
#define SCREENBACKUP *(uint32_t *)0x800a21b0
#define VABP *(int *)0x800e4490
#define DECOMPRESS_ADDR 0x800c8868
#else
#define RNG *(uint16_t *)0x80092530
#define RELOAD *(uint8_t *)0x800cdf28
#define PASTBRIGHT *(uint8_t *)0x800a3876
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
extern Enemy *enemyDataPointers[];
extern uint8_t loadState; /*1=loading*/

#if BUILD == 1395
static void (*mode_A_Table[2])(Game *) = {0x8001ea28, 0x8001eb48};
#else
static void (*mode_A_Table[2])(Game *) = {0x8001fe78, 0x8001ff98};
#endif

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

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void SwapWeaponTexturesClut(Mega *megaP);

void LoadSigmaOverlay(int ovl);

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

void SwapTexture(bool sync)
{
    if (sync)
    {
        DrawSync(0); // Wait for Images & Clut to finish transfering before doing texture swap
    }

    void *p = DECOMPRESS_ADDR; // Decompressed Texture buffer (temporary storage)
    void *p2 = swapTexturePointer;
    RECT rect = {320, 256, 512, 8};

    for (size_t i = 0; i < 32; i++)
    {
        StoreImage2(&rect, p);
        LoadImage2(&rect, p2);

        MemoryCopy(p2, p, 0x2000);

        p2 = (int)p2 + 0x2000;
        rect.y += 8;
    }
    practice.page ^= 1;
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

    size_t screenLength = ((*(uint32_t *)0x1F80000C) - (*(uint32_t *)0x1F800008)); // getting screen count via pointers
    practice.state.screenSize = screenLength;
    MemoryCopy(SCREENBACKUP, *(uint32_t *)0x1F800008, screenLength);
}
void LoadState() //TODO: fix Wolf's slow motion & Brightness CLUT
{
    ThreadSleep(10); // Waiting before transfering

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

    if (practice.page != practice.state.page)
    {
        SwapTexture(false);
    }
    practice.page = practice.state.page;
    swapTextureFlag = practice.state.textureFlag;
    PASTBRIGHT = practice.state.pastBright;
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

    if (refightsBss)
    {
        MemoryCopy(maverickRefightBssAddresses[game.point - 2], BSS_ADDR, maverickRefightBssSizes[game.point - 2]);
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
}

void StateCheck(Game *gameP)
{
    int time = gameP->stageTime;
    int totalSeconds = time / 60;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    int frames = time % 60;

    DrawDebugText(4, 3, 0, "%2d:%2d:%2d", minutes, seconds, frames);

    if (loadState == 1)
    {
        DrawDebugText(4, 4, 2, "(LOADING)");
    }

    if (loadState != 1 && FADE_F == 0 && EXPO_F == 0)
    {
        if ((buttonsPressed & (PAD_L2 | PAD_R2 | PAD_SELECT)) != 0)
        {
            if ((buttonsHeld & (PAD_R2 + PAD_SELECT)) == (PAD_R2 + PAD_SELECT))
            {
                SaveState();
            }
            else if ((buttonsHeld & (PAD_L2 + PAD_SELECT)) == (PAD_L2 + PAD_SELECT) && practice.state.made)
            {
                LoadState();
            }
        }
    }
    mode_A_Table[gameP->mode2](gameP);
}

#undef RNG
#undef RELOAD
#undef PASTBRIGHT
#undef UPDATECLUT
#undef STARTSELECT_FLAG
#undef SCREENBACKUP
#undef VABP

#undef FADE_F
#undef EXPO_F

#undef DECOMPRESS_ADDR