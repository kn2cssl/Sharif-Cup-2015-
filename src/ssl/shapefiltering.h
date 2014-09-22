#ifndef SHAPEFILTERING_H
#define SHAPEFILTERING_H

#include <QObject>
#include <shape.h>
#include "QDebug"

class ShapeFiltering : public Shape
{
public:
    explicit ShapeFiltering();
    void shapeManager(std::string type);
    void plusFrameCount();
    void minusFrameCount();

    bool checked;
//    bool
    int frameCount;

private:
    int TRIcount;
    int RECTcount;
    int PENTAcount;
    int CIRcount;
    int Robotcount;

    bool shapeDetected;

    int shapeCount[5];


signals:

public slots:

};

#endif // SHAPEFILTERING_H
