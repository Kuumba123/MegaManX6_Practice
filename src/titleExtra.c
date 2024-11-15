#include <common.h>

static struct Title
{
    int8_t mode;
    int8_t mode2;
    int8_t mode3;
    int8_t mode4;

    int16_t timer;
};

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void ShowPracticeTitleText()
{
    DrawDebugText(10, 15, 0, "PRACTICE BETA");
    DrawDebugText(32, 26, 2, "@POGCHAMPGUY");
}

void TitlePressStartHook(struct Title *titleP)
{
    titleP->timer -= 1;
    if (titleP->timer == 1)
    {
        titleP->mode2 = 4;
        FadeOut(8);
    }
    ShowPracticeTitleText();
}