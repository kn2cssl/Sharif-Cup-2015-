    #include "renderarea.h"

    RenderArea::RenderArea(Soccer* sc) :
        QWidget()
    {
        _sc = sc;
        brush_field = new QBrush(QPixmap(":/resources/images/Field.jpg"));
        brush_ball = new QBrush(QColor::fromRgb(255,200,0),Qt::SolidPattern);
        brush_yrobot = new QBrush(QColor::fromRgb(255,255,0),Qt::SolidPattern);
        brush_brobot = new QBrush(QColor::fromRgb(50,50,255),Qt::SolidPattern);
        brush_region1 = new QBrush(QColor::fromRgb(204,0,0),Qt::SolidPattern);
        brush_region2 = new QBrush(QColor::fromRgb(204,255,255),Qt::SolidPattern);
        brush_holes = new QBrush(QColor::fromRgb(204,204,204),Qt::SolidPattern);

        brush_red_shape = new QBrush(QColor::fromRgb(255,0,0),Qt::SolidPattern);
        brush_blue_shape = new QBrush(QColor::fromRgb(0,0,255),Qt::SolidPattern);
        brush_green_shape = new QBrush(QColor::fromRgb(0,255,0),Qt::SolidPattern);
        brush_yellow_shape = new QBrush(QColor::fromRgb(255,255,0),Qt::SolidPattern);
        brush_cyan_shape = new QBrush(QColor::fromRgb(0,255,255),Qt::SolidPattern);
        brush_violet_shape = new QBrush(QColor::fromRgb(153,0,153),Qt::SolidPattern);
        brush_black_shape = new QBrush(QColor::fromRgb(0,0,0),Qt::SolidPattern);

        _timer.start(40);
        connect(&_timer,SIGNAL(timeout()), this, SLOT(refresh()));
    }

    void RenderArea::refresh()
    {
        update();
    }

    void RenderArea::paintEvent(QPaintEvent *)
    {
        QPainter painter(this);

        // Settings
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        // Draw Field
        painter.setBrush(*brush_field);
        painter.drawRect(0,0,740,540);



        painter.setBrush(*brush_field);
        painter.drawRect(CENTER_X,CENTER_Y-154,275,280); // Draw Camera region

        //      // //    Draw circle FOR MISION III
        if(_sc->wm->mission==3)
        {
            painter.setBrush(*brush_field);
            painter.drawRect(CENTER_X-25,CENTER_Y-175,350,350);

            painter.setPen(QColor::fromRgb(0,0,0));
            Vector2D center;
            center=_sc->wm->circularBorder.center();
            center.x /= WORLD_SCALE;
            center.y /= WORLD_SCALE;
            painter.drawEllipse(QPoint(center.x+CENTER_X,-center.y+CENTER_Y),175,175);//17+150+CENTER_X,-11+CENTER_Y),175,175);//200/2,200/2);
            painter.setPen(QColor::fromRgb(204,51,0));
            painter.drawEllipse(QPoint(center.x+CENTER_X,-center.y+CENTER_Y),175/2,175/2);
            //               H O L E S
            painter.setBrush(*brush_holes);
            Vector2D HoleCenter;
            HoleCenter=_sc->wm->goal1.center();
//            qDebug() << " CENTER HOLE : (" << _sc->wm->goal1.center().x << "," << _sc->wm->goal1.center().y << ")";
            HoleCenter.x /= WORLD_SCALE;
            HoleCenter.y /= WORLD_SCALE;
            //painter.setPen(QColor::fromRgb(0,0,0));
            painter.drawEllipse(QPoint(HoleCenter.x+CENTER_X,-HoleCenter.y+CENTER_Y),25/2,25/2);

            HoleCenter=_sc->wm->goal2.center();
//            qDebug() << " CENTER HOLE : (" << _sc->wm->goal2.center().x << "," << _sc->wm->goal2.center().y << ")";
            HoleCenter.x /= WORLD_SCALE;
            HoleCenter.y /= WORLD_SCALE;
            //painter.setBrush(*brush_holes);
            //painter.setPen(QColor::fromRgb(0,0,0));
            painter.drawEllipse(QPoint(HoleCenter.x+CENTER_X,-HoleCenter.y+CENTER_Y),25/2,25/2);

        }

        // FPS
        painter.drawText(20,20,QString::number(_fps.FPS()));

        painter.translate(CENTER_X,CENTER_Y);
        qDebug() << " MISION : " << _sc->wm->mission;
        // -------------------- Sharif Cup 2014 ---------------------\\
        //       // //   Draw Regions FOR MISION I
        if(_sc->wm->mission==1)
        {
            painter.setBrush(*brush_region1);
            QPoint tl1(int(_sc->wm->region1_tl.x/WORLD_SCALE),-int(_sc->wm->region1_tl.y/WORLD_SCALE));
            QPoint br1(int(_sc->wm->region1_br.x/WORLD_SCALE),-int(_sc->wm->region1_br.y/WORLD_SCALE));
            QRect region1(tl1,br1);
            Vector2D mean1((tl1.x()+br1.x())/2,(tl1.y()+br1.y())/2);
//            painter.drawRect(-50,50,100,100);
//            painter.drawRect(tl1.x/WORLD_SCALE,tl1.y/WORLD_SCALE,br1.x/WORLD_SCALE,br1.y/WORLD_SCALE);
            painter.drawRect(region1);
            painter.setPen(QColor::fromRgb(0,0,0));
            painter.drawText(mean1.x-20,mean1.y,/*-65,180,*/"Reg 1");

            QPoint tl2(int(_sc->wm->region2_tl.x/WORLD_SCALE),-int(_sc->wm->region2_tl.y/WORLD_SCALE));
            QPoint br2(int(_sc->wm->region2_br.x/WORLD_SCALE),-int(_sc->wm->region2_br.y/WORLD_SCALE));
            QRect region2(tl2,br2);
            Vector2D mean2((tl2.x()+br2.x())/2,(tl2.y()+br2.y())/2);
//            qDebug() << " T.L : (" << tl2.x << "," <<tl2.y << ") , B.R : (" << br2.x << "," << br2.y<<") , MEAN : ( " << mean2.x << "," << mean2.y << ")";

            painter.setBrush(*brush_region2);
            painter.drawRect(region2);
//            painter.drawRect(tl2.x/WORLD_SCALE,tl2.y/WORLD_SCALE,br2.x/WORLD_SCALE,br2.y/WORLD_SCALE);
//            painter.drawRect();
            painter.setPen(QColor::fromRgb(0,0,0));
            painter.drawText(mean2.x-20,mean2.y,/*-65,180,*/"Reg 2");

            DrawShapes_sharif(_sc->wm->shapes4Region1,painter);
            DrawShapes_sharif(_sc->wm->shapes4Region2,painter);


            //qDebug()<<"NUMBER:"<<_sc->wm->shapes4Region1.size();
            //        for(int i=0;i<_sc->wm->shapes4Region1.size();i++)
            //        {
            //            painter.setPen(QColor::fromRgb(150,100,0));
            //            painter.setBrush(*brush_shape);
            //            //qDebug()<<"robot"<<i<<_sc->wm->shapes4Region1.at(i).position.x/WORLD_SCALE<<","<<_sc->wm->shapes4Region1.at(i).position.y/WORLD_SCALE;
            //            painter.drawEllipse(QPoint(_sc->wm->shapes4Region1.at(i).position.x/WORLD_SCALE,-_sc->wm->shapes4Region1.at(i).position.y/WORLD_SCALE),
            //                                _sc->wm->shapes4Region1.at(i).roundedRadios,_sc->wm->shapes4Region1.at(i).roundedRadios);
            //        }

            //        for(int i=0;i<_sc->wm->shapes4Region2.size();i++)
            //        {
            //            painter.setPen(QColor::fromRgb(150,100,0));
            //            painter.setBrush(*brush_shape2);
            //            //qDebug()<<"robot"<<i<<_sc->wm->shapes4Region1.at(i).position.x/WORLD_SCALE<<","<<_sc->wm->shapes4Region1.at(i).position.y/WORLD_SCALE;
            //            painter.drawEllipse(QPoint(_sc->wm->shapes4Region2.at(i).position.x/WORLD_SCALE,-_sc->wm->shapes4Region2.at(i).position.y/WORLD_SCALE),
            //                                _sc->wm->shapes4Region2.at(i).roundedRadios,_sc->wm->shapes4Region2.at(i).roundedRadios);
            //        }
        }
        //      // //    Draw Line FOR MISION II
        if(_sc->wm->mission==2)
        {
            //    painter.drawLine(-50, 200,0,0);
            //    painter.drawLine(-50,-200,0,0);
            qDebug() << _sc->wm->borders.size() << " BORDER SIZE       !!!! " ;
            for(int i=0;i<_sc->wm->borders.size();i++)
            {
                Vector2D orgn(_sc->wm->borders.at(i).origin().x/10,-_sc->wm->borders.at(i).origin().y/10);
                Vector2D trmnl(_sc->wm->borders.at(i).terminal().x/10,-_sc->wm->borders.at(i).terminal().y/10);
                qDebug() << " ORIGIN : (" << orgn.x << "," <<orgn.y << ") , TERMINAL : (" << trmnl.x << "," << trmnl.y << ")" ;
                painter.drawLine(orgn.x,orgn.y,
                                 trmnl.x,trmnl.y);
                //painter.drawLine(-50,-200,0,0);
            }

            DrawShapes_sharif(_sc->wm->negativeShapes,painter);
            DrawShapes_sharif(_sc->wm->positiveShapes,painter);

            //qDebug()<<"NUMBER:"<<_sc->wm->negativeShapes.size();
            //        for(int i=0;i<_sc->wm->negativeShapes.size();i++)
            //        {
            //            painter.setPen(QColor::fromRgb(150,100,0));
            //            painter.setBrush(*brush_shape);
            //            //qDebug()<<"robot"<<i<<_sc->wm->shapes4Region1.at(i).position.x/WORLD_SCALE<<","<<_sc->wm->shapes4Region1.at(i).position.y/WORLD_SCALE;
            //            painter.drawEllipse(QPoint(_sc->wm->negativeShapes.at(i).position.x/WORLD_SCALE,-_sc->wm->negativeShapes.at(i).position.y/WORLD_SCALE),
            //                                _sc->wm->negativeShapes.at(i).roundedRadios,_sc->wm->negativeShapes.at(i).roundedRadios);
            //        }

            //        for(int i=0;i<_sc->wm->positiveShapes.size();i++)
            //        {
            //            painter.setPen(QColor::fromRgb(0,255,255));
            //            painter.setBrush(*brush_shape2);
            //            //qDebug()<<"robot"<<i<<_sc->wm->shapes4Region1.at(i).position.x/WORLD_SCALE<<","<<_sc->wm->shapes4Region1.at(i).position.y/WORLD_SCALE;
            //            painter.drawEllipse(QPoint(_sc->wm->positiveShapes.at(i).position.x/WORLD_SCALE,-_sc->wm->positiveShapes.at(i).position.y/WORLD_SCALE),
            //                                _sc->wm->positiveShapes.at(i).roundedRadios,_sc->wm->positiveShapes.at(i).roundedRadios);
            //        }

        }
//        DrawShapes_sharif();

        if(_sc->wm->theirRobot.IsValid) DrawShapes_sharif(_sc->wm->theirRobot,painter);



        // CENTERofField = (0,0)
        // Y_TOP_EDGE  =       Y_DOWN_EDGE = 470
        // X_LEFT_EDGE = 70      X_RIGHT_EDGE =670

        //-------------------- End Sharif Cup 2014 -------------------//

        // Draw Robots
        painter.setPen(QColor::fromRgb(0,0,0));

        painter.setBrush(*brush_yrobot);
        for(int i=0;i<12;i++)
            if(_sc->wm->oppRobot[i].isValid)
            {
                painter.drawChord(_sc->wm->oppRobot[i].pos.loc.x/WORLD_SCALE-ROBOT_R,-_sc->wm->oppRobot[i].pos.loc.y/WORLD_SCALE-ROBOT_R,ROBOT_D,ROBOT_D,ANGLE_TO_CHORD(_sc->wm->oppRobot[i].pos.dir));
                painter.drawText(_sc->wm->oppRobot[i].pos.loc.x/WORLD_SCALE-ROBOT_R,-_sc->wm->oppRobot[i].pos.loc.y/WORLD_SCALE-ROBOT_R,ROBOT_D,ROBOT_D,Qt::AlignCenter,QString::number(i));
            }

        painter.setBrush(*brush_brobot);
        for(int i=0;i<12;i++)
            if(_sc->wm->ourRobot[i].isValid)
            {
                painter.drawChord(_sc->wm->ourRobot[i].pos.loc.x/WORLD_SCALE-ROBOT_R,-_sc->wm->ourRobot[i].pos.loc.y/WORLD_SCALE-ROBOT_R,ROBOT_D,ROBOT_D,ANGLE_TO_CHORD(_sc->wm->ourRobot[i].pos.dir));
                painter.drawText(_sc->wm->ourRobot[i].pos.loc.x/WORLD_SCALE-ROBOT_R,-_sc->wm->ourRobot[i].pos.loc.y/WORLD_SCALE-ROBOT_R,ROBOT_D,ROBOT_D,Qt::AlignCenter,QString::number(i));
            }

        // Draw Ball
//        if(_sc->wm->ball.isValid)
//        {
//            painter.setPen(QColor::fromRgb(200,100,0));
//            painter.setBrush(*brush_ball);
//            painter.drawEllipse(QPoint(_sc->wm->ball.pos.loc.x/WORLD_SCALE,-_sc->wm->ball.pos.loc.y/WORLD_SCALE),BALL_R,BALL_R);
//        }

       for(int i=0;i<_sc->wm->balls.size();i++)
        {
            painter.setPen(QColor::fromRgb(200,100,0));
            painter.setBrush(*brush_ball);
            painter.drawEllipse(QPoint(_sc->wm->balls.at(i)->pos.loc.x/WORLD_SCALE,-_sc->wm->balls.at(i)->pos.loc.y/WORLD_SCALE),BALL_R,BALL_R);
        }

        // TEST

        //painter.drawChord(100-ROBOT_R,100-ROBOT_R,ROBOT_D,ROBOT_D,ANGLE_TO_CHORD(0));
        //painter.drawText(100-ROBOT_R,100-ROBOT_R,ROBOT_D,ROBOT_D,Qt::AlignCenter,"8");

        _fps.Pulse();
    }

    void RenderArea::DrawShapes_sharif(QList<Shape> list, QPainter &painter)
    {
        for(int k=0 ; k<list.size() ; k++)
        {
            painter.setPen(QColor::fromRgb(150,100,0));
            //painter.setBrush(*brush_shape2);
            if(list.at(k).color=="red")
            {
                painter.setBrush(*brush_red_shape);
            }
            else if(list.at(k).color=="blue")
            {
            painter.setBrush(*brush_blue_shape);
            }
            else if(list.at(k).color=="green")
            {
            painter.setBrush(*brush_green_shape);
            }
           else if(list.at(k).color=="yellow")
           {
            painter.setBrush(*brush_yellow_shape);
            }
            else if(list.at(k).color=="violet")
            {
            painter.setBrush(*brush_violet_shape);
            }
            else if(list.at(k).color=="black")
            {
            painter.setBrush(*brush_black_shape);
            }
            else if(list.at(k).color=="cyan")
            {
            painter.setBrush(*brush_cyan_shape);
            }

//            painter.drawEllipse(QPoint(_sc->wm->.at(i).position.x/WORLD_SCALE,-_sc->wm->shapes4Region2.at(i).position.y/WORLD_SCALE),
//            _sc->wm->shapes4Region2.at(i).roundedRadios,_sc->wm->shapes4Region2.at(i).roundedRadios);

            painter.drawEllipse(QPoint(list.at(k).position.x/WORLD_SCALE,-list.at(k).position.y/WORLD_SCALE),
                        list.at(k).roundedRadios,list.at(k).roundedRadios);

            double Xtext=(list.at(k).position.x/WORLD_SCALE - 3*list.at(k).roundedRadios/4);
            double Ytext=(-list.at(k).position.y/WORLD_SCALE );//+ list.at(k).roundedRadios);
            painter.drawText(Xtext,Ytext,QString::fromStdString(list.at(k).type));
        }
    }

    void RenderArea::DrawShapes_sharif(Shape rbt, QPainter &painter)
    {
        painter.setPen(QColor::fromRgb(150,100,0));
        painter.setBrush(*brush_black_shape);
        painter.drawEllipse(QPoint(rbt.position.x/WORLD_SCALE,-rbt.position.y/WORLD_SCALE),
                    rbt.roundedRadios,rbt.roundedRadios);

        double Xtext=(rbt.position.x/WORLD_SCALE - 3*rbt.roundedRadios/4);
        double Ytext=(-rbt.position.y/WORLD_SCALE );//+ list.at(k).roundedRadios);
        painter.drawText(Xtext,Ytext,QString::fromStdString(rbt.type));
    }

