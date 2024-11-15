#include <common.h>

#define StartingThread 0x8001cb3c

static struct DISC_FILE {
    char * fileName;
    int sectorSize;
    int dest;
};

static struct DISC_FILE files[] = {
    {"\\FONT.BIN;1",4,0x80101304},
    {"\\STATE.BIN;1",3,0x801f4e2c},
};

void LoadFile(struct DISC_FILE * file);

void LoadDebugCodeFile()
{
    for (size_t i = 0; i < 2; i++)
    {
        LoadFile(&files[i]);
    }
    NewThread(0, StartingThread);
}
#undef StartingThread