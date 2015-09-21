#ifndef TACTICSHARIFDEFENCE_H
#define TACTICSHARIFDEFENCE_H

#include "tactic.h"
#include "ball.h"

class TacticSharifDefence : public Tactic
{
    Q_OBJECT
public:
    explicit TacticSharifDefence(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void addData();
    void addHolePoints();
    void mergeData();
    void sortData();
    void addVel();
    void goOncircle2point(Vector2D pnt);
    void nearest2Opp();
    Vector2D findnearest2circle();
    Vector2D findnearesthole(Vector2D pnt);
    Vector2D AvoidtoEnterCircle(Circle2D Ci , Vector2D pnt, Vector2D finPOS);

private:
    QList<Ball *> balls;//R0;
    QList<Ball *> movingballs;
    QList<Ball *> lastBalls;
//    Vector2D hole1_V[8],hole2_V[8];
    //QList<Ball *> Aball;
    //QList<Vector2D> a4fSorted;
    //QList<Vector2D> agentsNegative;//R1;
    //QList<AgentsAndRegions> mergedList;
    //QList<Segment2D> segList;
    //QList<Vector2D> a4sSorted;
    //Rect2D region[2];
    //Rect2D region2;
    bool firstInit;
    bool IsInside,IsInside2,Accessible,oppIsInField,oppIsKhoraak,AllIn,AnyIn;
    bool InHole,Avoided;
    int state,state2;
    int index,towardH1,towardH2,towardHS;
    int loop;
    //int obs;
    //int goalRegion;
    int temp,temp2;
    int count;
    double Uangle1,Uangle2,Dangle1,Dangle2;
    Circle2D circularBorder,circularBorderOut,circularBorderin,hole1,hole2,circularMid,hole1_Offset,hole2_Offset;
    Line2D *ballVel;//b2h1,*b2h2;
    Ray2D ball2center;
    //Segment2D *testseg;
    //Segment2D *tseg;
    Segment2D *r2o; // Robot to Object
    Segment2D *c2o; // cCenter to Object
    //Ray2D c2b;
    //Segment2D *seg[2]; // 2 segment line
    Vector2D vec2goal,OppositeRobot,point,p2o,origin;

    Vector2D A;
    Vector2D B;//,c2o,origin,origin2;
    Vector2D vel,Opp;
};

#endif // TACTICSHARIFDEFENCE_H
