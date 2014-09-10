#ifndef BALL_H
#define BALL_H

#include "mobileobject.h"

class Ball : public MobileObject
{
    Q_OBJECT

public:
    explicit Ball();

    bool checked;
    int frameCount;

};

#endif // BALL_H
