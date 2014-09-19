#ifndef TACTICPUSH2GOAL_H
#define TACTICPUSH2GOAL_H

#include "tactic.h"

class TacticPush2Goal: public Tactic
{
    Q_OBJECT
public:
    explicit TacticPush2Goal(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void addData();
    void sortData();
    Vector2D GoOncircle(Vector2D center,double radius);//,Vector2D Object);
    Vector2D AvoidtoEnterCircle(Circle2D Ci , Vector2D pnt, Vector2D finPOS);
    void FindBall();
    void FindHole();
    void furtherByOpp();
    Vector2D KeepInField(RobotCommand rc);
    bool IsInCorner(Vector2D pnt,double margin);
    bool IsInmargins(Vector2D pnt,double margin);

private:
    QList<Ball *> balls;
    QList<Ball *> byOpp;
    double MAX_X , MAX_Y , MIN_X , MIN_Y , mean_x , mean_y ;
    bool firstInit;
    bool IsInside,unAccessible,AllIn,AnyIn,AllInCorner,AllUnAccessible;
    bool DontEnterCircle,oppIsValid,OppIsKhoraak;
    bool Avoided;
    int state,state2,statemargin;
    int index;
    int temp,temp2;
    int count;
    Circle2D circularBorder,circularBorderOut,circularBorderDANGER,circularBorder2,hole1,hole2,hole1_Offset,hole2_Offset;
    Segment2D *c2o; // cCenter to Object
    Vector2D vec2goal,diff2,opp;//origin,origin2;
    Vector2D point2;
};


#endif // TACTICPUSH2GOAL_H
