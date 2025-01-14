#include <common.h>

#if BUILD == 1395
#define TITLE_X 10
#define TITLE_Y 15
#define TITLE_CLUT 0
#define NAME_X 32
#define NAME_Y 26
#define NAME_CLUT 2
#else
#define TITLE_X 16
#define TITLE_Y 12
#define TITLE_CLUT 1
#define NAME_X 22
#define NAME_Y 21
#define NAME_CLUT 2
#endif

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
    DrawDebugText(TITLE_X, TITLE_Y, TITLE_CLUT, "PRACTICE BETA");
    DrawDebugText(NAME_X, NAME_Y, NAME_CLUT, "@POGCHAMPGUY");
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

#undef TITLE_X
#undef TITLE_Y
#undef TITLE_CLUT
#undef NAME_X
#undef NAME_Y
#undef NAME_CLUT