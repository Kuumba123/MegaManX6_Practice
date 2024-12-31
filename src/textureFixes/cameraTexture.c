#include <object.h>
#include "../practice.h"

extern uint32_t swapTextureFlag;

void CheckTexturePage(Object *objP)
{
    if (practice.page == 0)
    {
        swapTextureFlag = 1;
    }
    DeleteObject3(objP);
}