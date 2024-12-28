#include <common.h>
#include <object.h>

#if BUILD == 1395
#define ANIME_TBL_ADDR 0x800f124c
#else
#define ANIME_TBL_ADDR 0x800f2878
#endif

void MiscObject_2D_StartUp(Object * objP)
{
    objP->texCord = 0x1800;
    objP->clutCord = 0x780d;
    objP->sprtDataP = *(int*)0x1f800020 + ((int*)(*(int*)0x1f800020))[0];
    objP->priority = 3;
    objP->animeTableP = ANIME_TBL_ADDR;

    if (objP->stageVar == 0) //Player Select Text
    {
        posX = 0x5B0;
        posY = 0x6C;
        SetAnime(objP,0);
        objP->act = 1;
    }
    else if (objP->stageVar == 1) //Left/Right Arrows
    {
        posX = 0x5B0;
        posY = 0x7C;
        SetAnimeFrame(objP,1,game.refights[3]);
        objP->act = 2;
    }
    else //Player Select Text (X,Falcon,Zero etc)
    {
        objP->sprtDataP = *(int*)0x1f800020 + ((int*)(*(int*)0x1f800020))[5]; //Slot 5
        posX = 0x5B0;
        posY = 0x7C;
        if (game.refights[2] == 0)
        {
            objP->animeInfo.sprtFrame = game.var[0];
        }
        else
        {
            objP->animeInfo.sprtFrame = game.var[0] + game.refights[2] * 2 + 2;
        }
        objP->act = 3;
    }
    objP->display = true;
    objP->layer = 1;
}
void MiscObject_2D_AreaOptionText(Object * objP)
{
    if (game.refights[2] == 0)
    {
        objP->animeInfo.sprtFrame = game.var[0];
    }
    else
    {
        objP->animeInfo.sprtFrame = game.var[0] + game.refights[2] * 2 + 2;
    }
}