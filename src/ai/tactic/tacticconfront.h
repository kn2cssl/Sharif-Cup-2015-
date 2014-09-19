#ifndef TACTICCONFRONT_H
#define TACTICCONFRONT_H

#include "tactic.h"
#include "shape.h"

class TacticConfront: public Tactic
{
    Q_OBJECT
public:
    explicit TacticConfront(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void addData();
    void mergeData();
    void sortData();
    void addseg();
    Vector2D AvoidtoEnterCircle(Circle2D Ci , Vector2D pnt, Vector2D finPOS);
    Vector2D findnearest(Vector2D input);
    int findnearestObject(QList<Shape> list,Vector2D Pos);


private:
    QList<Vector2D> agentsPositive;//R0;
    //QList<Vector2D> a4fSorted;
    QList<Vector2D> agentsNegative;//R1;
    //QList<AgentsAndRegions> mergedList;
    QList<Segment2D> segList;
    //QList<Vector2D> a4sSorted;
    Rect2D region[2];
    //Rect2D region2;
    bool firstInit;
    bool IsOverTheLine;
    bool DoNotEnterOpposedField;
    bool CanKickOpp,OppIsInhisField,OppIsValid;
    int state;
    int index;
    int obs;
    int goalRegion;
    int temp,temp2;
    int count;
    Line2D *testline;
    Segment2D *testseg;
    Segment2D *tseg;
    Segment2D *r2o; // Robot to Object
    Segment2D *o2o; // Origin to Object
    //Segment2D *seg[2]; // 2 segment line
    Vector2D origin,origin2,rcpast,Opp;
    Circle2D ObsC;
};
#endif // TACTICCONFRONT_H
