#include <object.h>

#if BUILD == 1395
#define SPEC_ADDR 0x800F0B44
#define FRAMECOUNT *(int*)0x80097420
#else
#define SPEC_ADDR 0x800f21b8
#define FRAMECOUNT *(int*)0x80098ae0
#endif

static struct Title
{
    int8_t mode;
    int8_t mode2;
    int8_t mode3;
    int8_t mode4;

    int16_t timer;
};

static struct TitleText
{
    int16_t x;
    int16_t y;
    uint8_t sprite;
    uint8_t clutId;
};

extern struct Title title;

static void MiscObj_13_1(Object *objP)

{
    int iVar1;
    int iVar2;

    if (objP->stageVar == 6)
    {
        objP->priority = 2;
        iVar2 = (int)(char)title.mode3;
        iVar1 = iVar2;
        if (iVar2 < 0)
        {
            iVar1 = iVar2 + 3;
        }
        *(int16_t *)((int)&objP->y + 2) = ((struct TitleText*)(SPEC_ADDR))[iVar2].y;
        if (title.mode3 == 1)
        {
            (objP->animeInfo).sprtFrame = 1;
        }
        else
        {
            if ((char)title.mode3 < '\x02')
            {
                if (title.mode3 == 0)
                {
                    (objP->animeInfo).sprtFrame = 0;
                    goto LAB_ST0E_OVL__800eeb08;
                }
            }
            else if (title.mode3 == 2)
            {
                (objP->animeInfo).sprtFrame = 2;
                goto LAB_ST0E_OVL__800eeb08;
            }
            (objP->animeInfo).sprtFrame = 3;
        }
    }
LAB_ST0E_OVL__800eeb08:
    if (objP->stageVar == 5 || objP->stageVar == 4 || objP->stageVar == 7)
    {
        objP->display = 0;
        if ((FRAMECOUNT & 0x10U) != 0)
        {
            DisplayObject(objP);
        }
    }
    else
    {
        objP->display = 1;
        DisplayObject(objP);
    }
    return;
}

#undef SPEC_ADDR
#undef FRAMECOUNT