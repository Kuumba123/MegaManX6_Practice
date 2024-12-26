#include <object.h>

#if BUILD == 1395
#define ACT_1_ADDR 0x800ee408
#define ACT_2_ADDR 0x800ee410
#else
#define ACT_1_ADDR 0x800efa28
#define ACT_2_ADDR 0x800efa30
#endif
void MiscObject_2D_StartUp(Object * objP);
void MiscObject_2D_AreaOptionText(Object * objP);
static void * miscObject2D_Table[4] = {MiscObject_2D_StartUp, ACT_1_ADDR, ACT_2_ADDR, MiscObject_2D_AreaOptionText};
#undef ACT_1_ADDR
#undef ACT_2_ADDR