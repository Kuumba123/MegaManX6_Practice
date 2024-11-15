#include <stdint.h>

static struct TitleText{
    int16_t x;
    int16_t y;
    uint8_t sprite;
    uint8_t clutId;
};

static struct TitleText text[] = {
    {258, 142, 0, 0}, // GAME START
    {258, -69, 1, 0}, // CONTINUE
    {258, 158, 2, 0}, // OPTION
    {258, 182, 3, 0}, // TRAINING
    {258, 160, 4, 0}, // PRESS START BUTTON
    {220, 192, 5, 0}, // PRESS START or X BUTTON
    {258, 134, 0, 1}, // GAME START (Selected)
    {476, 192, 6, 0}
};
