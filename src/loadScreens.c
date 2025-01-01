
#include <common.h>
#include "practice.h"

static uint16_t ScreenSectorOffset[] = {
    0x158,0,    //ST00
    0x172,0,    //ST01
    0x193,0,    //ST02
    0x161,0,    //ST03
    0x18C,0x17E,//ST04
    0x160,0,    //ST05
    0x19F,0,    //ST06
    0x184,0,    //ST07
    0x1BF,0,    //ST08
    0,0,        //ST09
    0,0,        //ST0A
    0,0,        //ST0B
    0x139,0x162,//ST0C
    0,0,        //ST0D
    0,0,        //ST0E
    0,0,        //ST0F
    0x182,0,    //ST10
    0x180,0,    //ST11
    0x18E,0,    //ST12
    0xF7,0x118, //ST13
    0xE6,0x11F, //ST14
    0xE2,0x11B, //ST15
    0x105,0xFF  //ST16
};

static struct FileEntry
{
    uint32_t lba;
    uint32_t size;
};

extern struct FileEntry datFileInfo[246];

extern uint16_t LevelFileTable[46];

void LoadScreens()
{
    uint16_t fileId = LevelFileTable[game.stageId * 2 + game.mid];
    uint16_t offset = ScreenSectorOffset[game.stageId * 2 + game.mid];
    datFileInfo[73].lba = datFileInfo[fileId].lba + offset;
    datFileInfo[73].size = practice.state.screenSize;
    void * temp = freeArcP;
    ArcBinSeek(73,*(uint32_t *)0x1F800008);
    FileCollect();
    freeArcP = temp;
}