#include <common.h>

//Starts at Mode 4
void AreaSelectInit(Game * gameP);
void AreaSelect(Game * gameP);
void AreaSelectCoolDown(Game * gameP);
void AreaSelected(Game * gameP);
void AreaDetermine(Game* gameP);
static void * miscObject2D_Table[] = {AreaSelectInit, AreaSelect, AreaSelectCoolDown, AreaSelected,AreaDetermine};