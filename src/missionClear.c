#include <common.h>
#include <object.h>

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void MissionCleared(Game *gameP)
{
    gameP->stageSelectMode = 5;
    if (gameP->exitType == 0)
    {
        gameP->mode = 3;
        gameP->clearedStages = 0;
        return;
    }

    if (gameP->mode2 == 0)
    {
        gameP->mode2 = 1;
        FadeIn(9);
        return;
    }
    else if (gameP->mode2 == 1)
    {
        if (fadeDirection != 0)
        {
            return;
        }
        gameP->mode2 = 3;
    }
    else
    {
        while (true)
        {
            int time = gameP->stageTime;
            int totalSeconds = time / 60;
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;
            int frames = time % 60;

            DrawDebugText(6, 12, 0, "YOUR CLEAR TIME WAS - %2d:%2d:%2d", minutes, seconds, frames);
            DrawDebugText(8, 14, 0, "PRESS ANY BUTTON TO\nRETURN TO STAGE SELECT");

            if (buttonsPressed != 0)
            {
                gameP->mode2 = 0;
                gameP->mode = 3;
                gameP->clearedStages = 0;
                return;
            }
            ThreadSleep(1);
        }
    }
}