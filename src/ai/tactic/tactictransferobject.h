#ifndef TACTICTRANSFEROBJECT_H
#define TACTICTRANSFEROBJECT_H

#include "tactic.h"
#include "shape.h"

struct AgentsAndRegions
{
    Vector2D pos;
    int goalRegion;
};

class TacticTransferObject : public Tactic
{
    Q_OBJECT
public:
    explicit TacticTransferObject(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void addData();
    void mergeData();
    void sortData();
    Vector2D KeepInField(RobotCommand rc);
    Vector2D AvoidtoEnterCircle(Circle2D Ci , Vector2D pnt, Vector2D finPOS);
    int findnearestObject(QList<Shape> list,Vector2D Pos);
    bool IsInmargins(Vector2D pnt,double margin);

private:
    QList<Vector2D> agentsR0;
    //QList<Vector2D> a4fSorted;
    QList<Vector2D> agentsR1;
    QList<AgentsAndRegions> mergedList;
    QList<Shape> mergedShapeList;
    //QList<Vector2D> a4sSorted;
    Rect2D region[2];

    Vector2D rcpast;
    Circle2D ObsC;
    double MAX_X , MAX_Y , MIN_X , MIN_Y , mean_x , mean_y ;
    //Rect2D region2;
    bool firstInit,Avoided;
    int state,statemargin;
    int index;
    int goalRegion;
    int temp;
};

#endif // TACTICTRANSFEROBJECT_H
