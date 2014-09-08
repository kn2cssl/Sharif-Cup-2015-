#ifndef PLAYSTALL_H
#define PLAYSTALL_H

#include "play.h"

class PlayStall: public Play
{
    Q_OBJECT
public:
    explicit PlayStall(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    //virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticHalt* tHalt;

};
#endif // PLAYSTALL_H
