#include <common.h>

void StateCheck(Game * gameP);

static void (*mode_A_Table[2])(Game*) = {StateCheck,StateCheck};