#ifndef SHARIFVISION_H
#define SHARIFVISION_H

#include "sharifreceiver.h"
#include "worldmodel.h"
#include <Prototype_Messages/GameGround.pb.h>
#include "shapefiltering.h"

class SharifVision : public SharifReceiver
{
    Q_OBJECT
public:
    SharifVision(QString ip, int port, WorldModel *wm, QObject *parent = 0);

private:
    QTime _time;
    WorldModel *_wm;

    QList<ShapeFiltering> shapes;

    QList<ShapeFiltering> good;
    QList<ShapeFiltering> candidate;

    void parse(outputPacket &msg);
    void addToRegion1(ShapeFiltering input);
    void addToRegion2(ShapeFiltering input);
    void addToNegative(ShapeFiltering input);
    void addToPositive(ShapeFiltering input);
    void addToRobot(ShapeFiltering input);

    int findNearestShape(ShapeFiltering goal_shape, QList<ShapeFiltering> shape_list);

private slots:
    void readPendingPacket(QByteArray data, QString ip, int port);
};

#endif // SHARIFVISION_H
