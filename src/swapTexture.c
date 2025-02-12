#include <common.h>
#include <gpu.h>

extern uint32_t swapTextureFlag;

uint32_t SwapTexture();

void SwapTextureCheck(){
    if (swapTextureFlag == 1)
    {
        swapTextureFlag = SwapTexture();
    }
    PutDrawEnv(&drawP->drawenv);
}