#include <common.h>

extern int * StageLayoutPointers[];
extern int * CutsceneLayoutPointers[];

void FlushScreen(int i);

void LoadLayout()
{
    int *p;

    if (game.stageId == 0xB)
    {
        p = CutsceneLayoutPointers[game.point];
    }else{
        p = StageLayoutPointers[game.stageId * 2 + game.mid];
    }
    *(int*)0x1f800004 = p; //layout pointer

    FlushScreen(-1);
}

