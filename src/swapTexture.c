#include <common.h>

extern uint32_t swapTextureFlag;

uint32_t SwapTexture(bool sync);

void SwapTextureCheck(){
    if (swapTextureFlag == 1)
    {
        swapTextureFlag = SwapTexture(true);
    }
}