#include <gpu.h>

extern RECT clutRect;
extern void *clutPointer;
extern void *backupClut;
extern uint8_t updateClut;

void DumpClut2()
{
    if (updateClut != 0)
    {
        if ((updateClut & 1) != 0)
        {
            LoadImage(&clutRect, clutPointer);
        }
        if ((updateClut & (2 + 8)) != 0)
        {
            LoadImage(&clutRect, &backupClut);
        }
        updateClut = 0;
    }
}