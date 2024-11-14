#include <common.h>

extern uint32_t swapTextureFlag;

void SwapTexture(bool sync);

void SwapTextureCheck(){
    if (swapTextureFlag == 1)
    {
        SwapTexture(true);
        swapTextureFlag = 2;
    }
}