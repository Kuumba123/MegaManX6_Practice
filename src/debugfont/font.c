#include <common.h>
#include <gpu.h>
#include <stdio.h>
#include <stdarg.h>

#define _TAB_SIZE_ 2
#define MAX_CHAR_COUNT 0xDE
#define FONT8X8_FILE_ID 73

extern SPRT_8 textRects[MAX_CHAR_COUNT];

int debugTextCount = 0;
char hexDigits[] = "0123456789ABCDEF";
char lowerhexDigits[] = "0123456789abcdef";
DR_TPAGE static debugfontTPage = {0, 0xE1000014};

void LoadDebugFont()
{
    FileCollect();
    ArcSeek(FONT8X8_FILE_ID, 0, 0); // FONT8X8.ARC
    printf("Loading FONT8X8.ARC\n");
    FileCollect();
    RECT FONT8X8_TextureRECT = {
        0x100,
        0x1E0,
        0x40,
        0x18};
    RECT FONT8X8_ClutRECT = {
        0x100,
        0x1FF,
        0x40,
        1};
    LoadImage(&FONT8X8_TextureRECT, *((u_long *)0x1f800008));
    LoadImage(&FONT8X8_ClutRECT, *((u_long *)0x1f80000c));
}
void DrawChar(char letter, int destX, int destY, uint8_t clut) //Lower Case not Supported for X6
{
    if (debugTextCount < MAX_CHAR_COUNT && letter != ' ')
    {
        letter -= 0x20;
        SPRT_8 *sp = &textRects[debugTextCount];
        setRGB0(sp, 128, 128, 128);
        setXY0(sp, destX, destY); // 32 Chars per row
        setUV0(sp, (letter & 0x1F) * 8, 0xE0 + (letter / 32) * 8);
        setClut(sp, 0x100 + clut * 16, 0x1FF);
        setSprt8(sp);
        catPrim(sp, sp + 1);
        debugTextCount += 1;
    }
}
void static ReverseString(char *str, int length)
{
    for (int j = 0; j < length / 2; ++j)
    {
        char temp = str[j];
        str[j] = str[length - j - 1];
        str[length - j - 1] = temp;
    }
}
int static DrawString(int destX, int destY, int clut, char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        DrawChar(str[i], destX, destY, clut);
        i++;
        destX += 8;
    }
    return i;
}
void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...)
{
    va_list args;
    va_start(args, textP);
    char letter;
    int destX = x * 8;
    int destY = y * 8;
    int startX = destX;

    while ((letter = *textP) != '\0')
    {
        if (letter == '\n')
        {
            textP++; // Move to the next character
            destX = startX;
            destY += 8;
        }
        else if (letter == '\t')
        {
            textP++;
            destX += _TAB_SIZE_ * 8;
        }
        else if (letter == '%')
        {
            char str[12];
            int pad;
            if (textP[1] >= '0' && textP[1] <= '9')
            {
                pad = textP[1] - '0';
                textP++;
            }
            else
            {
                pad = 1;
            }
            for (size_t i = 0; i < pad; i++)
            {
                str[i] = '0';
            }
            str[pad] = '\0';

            if (textP[1] == 'x' || textP[1] == 'X')
            {
                int i = 0;
                uint num = va_arg(args, uint);

                if (num != 0)
                {
                    while (num != 0)
                    {
                        uint remainder = num % 16;
                        if (textP[1] == 'X')
                        {
                            str[i] = hexDigits[remainder];
                        }
                        else
                        {
                            str[i] = lowerhexDigits[remainder];
                        }
                        i += 1;
                        num /= 16;
                    }
                }

                int length;
                if (i > pad)
                {
                    str[i] = '\0';
                    length = i;
                }
                else
                {
                    length = pad;
                }

                ReverseString(&str, length);
                destX += DrawString(destX, destY, clut, &str) * 8;
            }
            else if (textP[1] == 'd' || textP[1] == 'i')
            {
                int i = 0;
                int num = va_arg(args, int);

                if (num == -2147483648)
                {
                    DrawString(destX, destY, clut, "-2147483648");
                    destX += 11;
                    goto KeepGoing;
                }
                else if (num < 0)
                {
                    DrawChar('-', destX, destY, clut);
                    destX += 8;
                    num = -num;
                }

                if (num != 0)
                {
                    while (num != 0)
                    {
                        int digit = num % 10;
                        str[i++] = digit + '0';
                        num /= 10;
                    }
                }
                else
                {
                    str[i++] = '0';
                }
                int length;
                if (i > pad)
                {
                    str[i] = '\0';
                    length = i;
                }
                else
                {
                    length = pad;
                }

                ReverseString(str, length);

                destX += DrawString(destX, destY, clut, &str) * 8;
            }
            else if (textP[1] == 'u')
            {
                int i = 0;
                uint num = va_arg(args, uint);

                if (num != 0)
                {
                    while (num != 0)
                    {
                        unsigned int digit = num % 10;
                        str[i++] = digit + '0';
                        num /= 10;
                    }
                }
                int length;
                if (i > pad)
                {
                    str[i] = '\0';
                    length = i;
                }
                else
                {
                    length = pad;
                }
                ReverseString(&str, length);
                destX += DrawString(destX, destY, clut, &str) * 8;
            }
            else if (textP[1] == 's')
            {
                char *p = va_arg(args, char *);
                while ((letter = *p) != '\0')
                {
                    DrawChar(letter, destX, destY, clut);
                    p += 1;
                    destX += 8;
                }
            }
            else if (textP[1] == '%')
            {
                DrawChar('%', destX, destY, clut);
                destX += 8;
            }
            else if (textP[1] == 'c')
            {
                char c = (char)va_arg(args, int);
                DrawChar(c, destX, destY, clut);
                destX += 8;
            }
        KeepGoing:
            textP += 2;
        }
        else
        {
            DrawChar(letter, destX, destY, clut);
            textP++; // Move to the next character
            destX += 8;
        }
    }
    va_end(args);
}
void DebugTextFlush()
{
    DrawSync(0);

    if (debugTextCount != 0)
    {
        textRects[debugTextCount - 1].tag = 0x03FFFFFF;
        debugTextCount = 0;
        catPrim(&debugfontTPage, &textRects[0]);
        setlen(&debugfontTPage, 1);
        DrawOTag2(&debugfontTPage);
    }
}
#undef _TAB_SIZE_
#undef FONT8X8_FILE_ID