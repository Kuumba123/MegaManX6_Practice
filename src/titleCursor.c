#include <common.h>

static struct Title
{
    int8_t mode;
    int8_t mode2;
    int8_t mode3;
    int8_t mode4;

    int16_t timer;
};

void ShowPracticeTitleText();

void TitleCursor(struct Title *titleP)
{
    if (titleP->mode3 == 1)
    {
        if ((buttonsPressed & PAD_UP) != 0)
        {
            titleP->mode3 = 0;
        }else{
            titleP->mode3 = 2;
        }
    }
    ShowPracticeTitleText();
}