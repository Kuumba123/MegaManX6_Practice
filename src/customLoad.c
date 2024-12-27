#include <common.h>
#include <misc.h>

void LoadCustomRoute(Game *gameP)
{
    gameP->slowMotion = gameP->stageId;
    gameP->maverickShow = gameP->mid;

    gameP->mode = 6;
    gameP->stageId = 0xD;
    gameP->mid = 1;
    LoadLevel();
    SetupScreen(0);
}