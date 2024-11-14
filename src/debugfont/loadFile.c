#include <common.h>

static struct DISC_FILE {
    char * fileName;
    int sectorSize;
    int dest;
};

void LoadFile(struct DISC_FILE * file)
{
    CdlFILE cdl;
    u_char results[8];

    printf("Loading %s\n", file->fileName);

    CdSearchFile(&cdl, file->fileName);
    CdControl(CdlSeekL, & cdl.pos, &results);
    CdRead(file->sectorSize, file->dest, 0x80);
    CdReadSync(0, &results);

    while (CdControl(CdlPause, 0, 0) == 0)
    {
    }
    
}