#include <common.h>
#include <gpu.h>
#include <layer.h>
#include "practice.h"

#if BUILD == 1395
#define LEVEL_OVL_ADDR 0x800E9860
#define DECOMPRESS_ADDR 0x800C8868
#define UPDATECLUT *(uint8_t *)0x800c4560
#define SCREENBACKUP *(uint32_t *)0x800a21b0
#else
#define LEVEL_OVL_ADDR 0x800EAE90
#define DECOMPRESS_ADDR 0x800C9F28
#define UPDATECLUT *(uint8_t *)0x800c5c20
#define SCREENBACKUP *(uint32_t *)0x800a3870
#endif

static struct Size
{
    uint8_t width;
    uint8_t height;
};

extern bool disableExtraFiles;

extern uint16_t LevelOvlTable[46];
extern uint16_t LevelFileTable[46];
extern uint16_t X_ClutFileTable[46];
extern uint16_t Zero_ClutFileTable[46];
extern uint16_t LevelTextFileTable[46];
extern uint16_t NightmareTextFileTable[8][2];
extern struct Size LevelLayoutSizeTable[46];
extern bool LevelExtraFilesTable[46];
extern uint16_t BossMugShotFileTable[24];

//Object Function Pointer Tables
extern void* LevelMainObjectsFunction_Table[46];
extern void* LevelVisualObjectsFunction_Table[46];
extern void* LevelShotObjectsFunction_Table[46];
extern void* LevelEffectObjectsFunction_Table[46];
extern void* LevelMiscObjectsFunction_Table[46];
extern void* LevelItemObjectsFunction_Table[46];
extern void* LevelQuadObjectsFunction_Table[46];

int GetPlayerMugShotId();
void MemoryCopy(void *dest, const void *src, size_t size);

void LoadLevel()
{
    int stageId = game.stageId;
    int mid = game.mid;

    BinSeek(LevelOvlTable[stageId * 2 + mid],LEVEL_OVL_ADDR);
    FileCollect2();

    *(int*)0x1F800080 = LevelMainObjectsFunction_Table[stageId * 2 + mid];
    *(int*)0x1F800084 = LevelVisualObjectsFunction_Table[stageId * 2 + mid];
    *(int*)0x1F800088 = LevelShotObjectsFunction_Table[stageId * 2 + mid];
    *(int*)0x1F80008C = LevelEffectObjectsFunction_Table[stageId * 2 + mid];
    *(int*)0x1F800090 = LevelMiscObjectsFunction_Table[stageId * 2 + mid];
    *(int*)0x1F800094 = LevelItemObjectsFunction_Table[stageId * 2 + mid];
    *(int*)0x1F800098 = LevelQuadObjectsFunction_Table[stageId * 2 + mid];

    if (stageId >= 0xD && stageId <= 0xF)
    {
        ArcSeek(LevelFileTable[stageId * 2 + mid],0,0);
    }
    else
    {
        ArcSeek(LevelFileTable[stageId * 2 + mid],3,0);
    }
    DrawLoad(false,false);

    if (game.player == 0)
    {
        ArcSeek(X_ClutFileTable[stageId * 2 + mid],1,0);
    }
    else
    {
        ArcSeek(Zero_ClutFileTable[stageId * 2 + mid],1,0);
    }

    if (stageId > 0x12)
    {
        LevelTextFileTable[stageId * 2 + game.player] = NightmareTextFileTable[(stageId - 0x13) * 2 + game.mid][game.player];
    }
    if (LevelTextFileTable[stageId * 2 + game.player] == 0)
    {
        DrawLoad(false,true);
    }
    else
    {
        DrawLoad(false,false);
        ArcSeek(LevelTextFileTable[stageId * 2 + game.player],1,0);
        DrawLoad(false,true);
    }

    size_t screenLength = ((*(uint32_t *)0x1F80000C) - (*(uint32_t *)0x1F800008)); // getting screen count via pointers
    practice.state.screenSize = screenLength;
    SCREENBACKUP = freeArcP;
    //Get Rid of this line for save states to carry over between deaths
    MemoryCopy(freeArcP,*(uint32_t *)0x1F800008,screenLength);
    freeArcP = (int)freeArcP + screenLength;

    layoutWidth = LevelLayoutSizeTable[stageId * 2 + mid].width;
    layoutHeight = LevelLayoutSizeTable[stageId * 2 + mid].height;
    layoutSize = layoutWidth * layoutHeight;
    UPDATECLUT = 1;

    if (disableExtraFiles == false && LevelExtraFilesTable[stageId * 2 + mid] != false)
    {
        int id = GetPlayerMugShotId();
        ArcBinSeek(id * 2 + 0xA3, DECOMPRESS_ADDR);
        FileCollect();

        uint8_t * p = DECOMPRESS_ADDR;
        RECT rect = {0x180,0,0x40,0x10};

        for (size_t i = 0; i < 4; i++)
        {
            LoadImage(&rect,p);
            p += 0x800;
            rect.x += 0x40;
        }
        ArcBinSeek(id * 2 + 0xA4, *(int*)0x1F8000A8);
        FileCollect();

        if (stageId > 0x12)
        {
            uint16_t val;
            if (game.bonusBoss == 0)
            {
                val = 0xC1;
            }
            else if (game.bonusBoss == 1)
            {
                val = 0xC5;
            }
            else
            {
                val = 0xC3;
            }
            BossMugShotFileTable[stageId] = val;
        }
        uint16_t file = BossMugShotFileTable[stageId];
        if (file != 0)
        {
            ArcBinSeek(file,DECOMPRESS_ADDR);
            FileCollect();
            ArcBinSeek(file + 1,(*(int*)0x1F8000A8) + 0x400);
            FileCollect();
        }
    }
}


#undef LEVEL_OVL_ADDR
#undef DECOMPRESS_ADDR
#undef UPDATECLUT
#undef SCREENBACKUP