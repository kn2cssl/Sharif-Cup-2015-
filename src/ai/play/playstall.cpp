#include "playstall.h"

PlayStall::PlayStall(WorldModel *worldmodel, QObject *parent) :
    Play("PlayStall", worldmodel, parent)
{
   tHalt = new TacticHalt(wm);
}

int PlayStall::enterCondition()
{
    if(wm->mission == 0)
        return 100000;
    else
        return 0;
}

void PlayStall::execute()
{
    tactics[3] = tHalt;
    return ;

}
