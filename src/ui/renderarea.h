#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QList>
#include <QDebug>
#include "shape.h"
#include "soccer.h"
#include "fpscounter.h"

#define Pi 3.141592653589793238

#define WORLD_SCALE 10
#define CENTER_X 370
#define CENTER_Y 270

#define ROBOT_R 9
#define ROBOT_D 18

#define BALL_R 4

#define ANGLE_TO_CHORD(a) 16*((a)*180/Pi+40), 16*280

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(Soccer *sc);
private:
    Soccer* _sc;
    QBrush *brush_field, *brush_ball, *brush_yrobot, *brush_brobot, *brush_region1, *brush_region2
    , *brush_holes,*brush_red_shape,*brush_blue_shape,*brush_green_shape,*brush_yellow_shape,*brush_cyan_shape,
    *brush_violet_shape,*brush_black_shape;
    QTimer _timer;
    FPSCounter _fps;

signals:

public slots:
    void refresh();
    void paintEvent(QPaintEvent *);
    void DrawShapes_sharif(QList<Shape> list,QPainter &painter);
    void DrawShapes_sharif(Shape rbt,QPainter &painter);

};

#endif // RENDERAREA_H
