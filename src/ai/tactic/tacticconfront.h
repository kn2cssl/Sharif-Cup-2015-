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

    bool IsInmargins(Vector2D pnt,double margin);


private:
    QList<Shape> agentsPositive;//R0;
    //QList<Vector2D> a4fSorted;
    QList<Shape> agentsNegative;//R1;
    //QList<AgentsAndRegions> mergedList;
    QList<Segment2D> segList,seglistOffset;
    //QList<Vector2D> a4sSorted;
    Rect2D region[2];
    //Rect2D region2;
    bool firstInit;
    bool IsOverTheLine;
    bool IsOverThesafeline;
    bool DoNotEnterOpposedField;
    bool CanKickOpp,OppIsInhisField,OppIsValid;
    bool reach;
    bool IsInmargin;
    Circle2D ObsC;
    double MAX_X , MAX_Y , MIN_X , MIN_Y , mean_x , mean_y ;
    int state,statemargin;
    int nindex,pindex,rad;
    int obs;
    int goalRegion;
    int temp,temp2;
    int count;
    int counter=0;
    Line2D *testline;
    Segment2D *testseg;
    Segment2D *tseg;
    Segment2D *r2o; // Robot to Object
    Segment2D *o2o; // Origin to Object
    Segment2D *o3o;// origin to object
    Segment2D *safeline; //safeorigin1 to safeorigine2
    //Segment2D *seg[2]; // 2 segment line
    Vector2D origin,origin2,rcpast,Opp,point2,safeorigine2,safeorigine1;
    Vector2D OffsetforSeg;
    Vector2D unegative;
    Vector2D lastunegative=origin2;

    //shahin

   // bool Rflag=false;

    //shahout

};
#endif // TACTICCONFRONT_H
