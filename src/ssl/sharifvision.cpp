#include "sharifvision.h"

SharifVision::SharifVision(QString ip, int port, WorldModel *wm, QObject *parent) :
    SharifReceiver(ip,port,parent),
    _wm(wm)
{
    _time.start();
    connect(this, SIGNAL(newSharifPacket(QByteArray,QString,int)), this, SLOT(readPendingPacket(QByteArray,QString,int)));

    // Log
    qDebug() << "SharifVision Initialization...";
    qDebug() << "IP: " << ip;
    qDebug() << "Port: " << port;
}

void SharifVision::parse(outputPacket &msg)
{
    if(msg.type() == 0)
    {
        _wm->mission = msg.mission();
        _wm->numberOfShapes = msg.numberofshape();

        if(msg.mission() == 1)
        {
            Vector2D tl1(msg.mission1_region1_tl_x(),msg.mission1_region1_tl_y());
            Vector2D br1(msg.mission1_region1_br_x(),msg.mission1_region1_br_y());
            Rect2D region1(tl1,br1);

            Vector2D tl2(msg.mission1_region2_tl_x(),msg.mission1_region2_tl_y());
            Vector2D br2(msg.mission1_region2_br_x(),msg.mission1_region2_br_y());
            Rect2D region2(tl2,br2);

            _wm->region1 = region1;
            _wm->region2 = region2;

            Vector2D end(msg.mission1_end_x(),msg.mission1_end_y());
            _wm->endPoint = end;
        }
        else if(msg.mission() == 2)
        {
            for(int i=0;i<msg.mission2_lines().size();i++)
            {
                outputPacket_line temp_seg = msg.mission2_lines(i);
                Segment2D seg(Vector2D(temp_seg.start_x(),temp_seg.start_y()),Vector2D(temp_seg.end_x(),temp_seg.end_y()));
                _wm->borders.push_back(seg);
            }


            Vector2D end(msg.mission2_end_x(),msg.mission2_end_y());
            _wm->endPoint = end;
        }
        else if(msg.mission() == 3)
        {
            _wm->isAttacker = msg.mission3_isattacker();

            Vector2D borderCenter(msg.mission3_circularborde_x(),msg.mission3_circularborde_y());
            Circle2D cirBorder(borderCenter,1750);
            _wm->circularBorder = cirBorder;

            Vector2D goal1_center(msg.mission3_goal1_x(),msg.mission3_goal1_y());
            //            Vector2D goal1_center(1500,1700/4);
            Circle2D goal1(goal1_center,250);
            _wm->goal1 = goal1;
            //            _wm->oppRobot[0].pos.loc = goal1_center;
            //            _wm->oppRobot[0].isValid=true;

            Vector2D goal2_center(msg.mission3_goal2_x(),msg.mission3_goal2_y());
            //            Vector2D goal2_center(1500,-1700/4);
            Circle2D goal2(goal2_center,250);
            _wm->goal2 = goal2;
            //            _wm->oppRobot[1].pos.loc = goal2_center;
            //            _wm->oppRobot[1].isValid=true;
        }

        //qDebug()<<"World Model Initialize";
    }
    else if(msg.type() == 1)
    {
        shapes.clear();
        _wm->clean();
        //qDebug()<<"Data Message Received...";

        _wm->numberOfShapes = msg.numberofshape();
        _wm->mission = msg.mission();
        ShapeFiltering temp;// = new ShapeFiltering();
        outputPacket_shape op;

        for(int i=0;i<msg.shapes_size();i++)
        {
            op = msg.shapes(i);
            string color = op.color();
            string type = op.type();
            Vector2D pos(op.position_x(),op.position_y());
            //            qDebug() << i << " : Pos : (" << pos.x << ","<< pos.y << ") , Color :" <<
            //                         QString::fromStdString(color) << " , Type  :" << QString::fromStdString(type) ;
            double rad = op.radios();
            temp.set(pos,rad,color,type);
            shapes.push_back(temp);
        }

        // Shape Filtering

        // Remove repeated
        for(int i=0;i<shapes.size()-1;i++)
        {
            for(int j=i+1;j<shapes.size();j++)
            {
                if(shapes.at(i).position.dist2(shapes.at(j).position) < 1000) // 10 cm
                    shapes.removeAt(j);
            }
        }
        //        QList <ShapeFiltering> GoodTemp_list=good;
        //        QList <ShapeFiltering> CandidateTemp_list=candidate;
        //        good=shapes;
        for(int i=0;i<shapes.size();i++)
        {
            //            //ShapeFiltering shf = shapes.at(i);
            int index = findNearestShape(shapes.at(i),good);
            //            qDebug() << "Index = " << index;

            ////            qDebug()<<"1";

            if(index == -1)
            {
                index = findNearestShape(shapes.at(i),candidate);

                if(index == -1)
                {
                    candidate.push_back(shapes.at(i));
                    //                list.push_back(shapes.at(i)->pos);
                    candidate.last().checked = true;
                    //                candidate.last()->seenAt(list, time, cid);
                    candidate.last().set(shapes.at(i).position,shapes.at(i).roundedRadios,shapes.at(i).color,shapes.at(i).type);
//                    candidate.last().shapeManager(shapes.at(i).type);
                }
                else
                {
                    //                    CandidateTemp_list.removeAt(index);
                    candidate[index].plusFrameCount();
                    candidate[index].checked = true;
                    float radius=candidate[index].roundedRadios+0.06*(shapes.at(i).roundedRadios-candidate[index].roundedRadios);
                    Vector2D posit;
                    posit.x=candidate[index].position.x+0.4*(shapes.at(i).position.x-candidate[index].position.x);
                    posit.y=candidate[index].position.y+0.4*(shapes.at(i).position.y-candidate[index].position.y);

                    candidate[index].set(posit,radius,shapes.at(i).color,shapes.at(i).type);
//                    candidate[index].shapeManager(shapes.at(i).type);
                    //                list.push_back(shapes.at(i)->pos);
                    //                candidate.at(index)->seenAt(list, time, cid);
                }
            }
            else
            {
                //good.at(index)->frameCount++;
                //                GoodTemp_list.removeAt(index);
                good[index].checked = true;
                float radius=good[index].roundedRadios+0.06*(shapes.at(i).roundedRadios-good[index].roundedRadios);
                Vector2D posit;
                posit.x=good[index].position.x+0.4*(shapes.at(i).position.x-good[index].position.x);
                posit.y=good[index].position.y+0.4*(shapes.at(i).position.y-good[index].position.y);

                good[index].set(posit,radius,shapes.at(i).color,shapes.at(i).type);
//                qDebug()<<"Before change : " << QString::fromStdString(good[index].type);
                good[index].shapeManager(shapes.at(i).type);
//                qDebug()<<"After : " << QString::fromStdString(good[index].type);
            }
        }


        for(int i=0;i<candidate.size();i++)
        {
            if(candidate.at(i).checked)
            {
                candidate[i].checked = false;

                if( candidate.at(i).frameCount > 30)
                {
                    good.push_back(candidate.at(i));
                    //                    qDebug() << " Befor Being good  : " << good.last().frameCount;
                    good.last().frameCount = 200;
                    //                    qDebug() << " after Being good  : " << good.last().frameCount;
                    candidate.removeAt(i);
                }
            }
            else
            {
                candidate[i].checked = false;
                candidate[i].minusFrameCount();
                if( candidate.at(i).frameCount <= 0)
                {
                    candidate.removeAt(i);
                }
            }
        }

        for(int i=0;i<good.size();i++)
        {
            if(good.at(i).checked)
            {
                good[i].checked = false;
            }
            else
            {
                good[i].checked = false;
                good[i].minusFrameCount();
                if( good.at(i).frameCount < 30)
                {
                    candidate.push_back(good.at(i));
                    good.removeAt(i);
                }
            }
        }

//        qDebug() << " GOOD SIZE = " << good.size() << "  SHAPES SIZE =  " << shapes.size();

        //        for(int i=0;i<good.size();i++)
        //        {
        //             _wm->balls.push_back(good.at(i));
        //                    qDebug() << " Frame Count = " << good.at(i)->frameCount ;
        //        }


        // End Of Shape Filtering

        if(msg.mission() == 1)
        {


            for(int i=0;i<good.size();i++)
            {
                //                    qDebug()<<"1";
                //                    ShapeFiltering temp;
                //                    temp.set(shapes.at(i)->position,shapes.at(i)->roundedRadios,shapes.at(i)->color,shapes.at(i)->type);
                if(good.at(i).color == "yellow" || good.at(i).color == "blue" || good.at(i).color == "violet")
                {
                    addToRegion1(good.at(i));
                }
                else if(good.at(i).color == "red" || good.at(i).color == "green" || good.at(i).color == "cyan")
                {
                    addToRegion2(good.at(i));
                }


            }
        }
        else if(msg.mission() == 2)
        {
            for(int i=0;i<good.size();i++)
            {
                if(good.at(i).type =="RECT" || good.at(i).type =="CIR")
                {
                    addToNegative(good.at(i));
                }
                else
                {
                    addToPositive(good.at(i));
                }
            }
        }
        else if(msg.mission() == 3)
        {

        }

        //        qDebug()<<"World Model Updated";
    }

}

void SharifVision::addToRegion1(ShapeFiltering input)
{
    if(true/**/)
    {
        Shape t;
        t.set(input.position,input.roundedRadios,input.color,input.type);
        _wm->shapes4Region1.push_back(t);
    }

}

void SharifVision::addToRegion2(ShapeFiltering input)
{
    if(true/**/)
    {
        Shape t;
        t.set(input.position,input.roundedRadios,input.color,input.type);
        _wm->shapes4Region2.push_back(t);
    }
}

// GAND E MOHSEN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void SharifVision::addToNegative(ShapeFiltering input)
{
    if(true)
    {
        Shape t;
        t.set(input.position,input.roundedRadios,input.color,input.type);
        _wm->negativeShapes.push_back(t);
    }
}

void SharifVision::addToPositive(ShapeFiltering input)
{
    if(true)
    {
        Shape t;
        t.set(input.position,input.roundedRadios,input.color,input.type);
        _wm->positiveShapes.push_back(t);
    }
}

int SharifVision::findNearestShape(ShapeFiltering goal_shape, QList<ShapeFiltering> shape_list)
{
    double dist = 100000000000;
    int index = -1;
    for(int i=0;i<shape_list.size();i++)
    {
        if(!shape_list.at(i).checked)
        {
            double dist2 = goal_shape.position.dist2(shape_list.at(i).position);
                    if(goal_shape.color == shape_list.at(i).color)
                    {
            if(dist2<dist)
            {
                if(dist2<1000000)
                {
                    dist = dist2;
                    index = i;
                }
            }
        }
                }
    }
    return index;
}



// END GAND E MOHSEN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void SharifVision::readPendingPacket(QByteArray data, QString ip, int port)
{
    outputPacket message;
    if(!message.ParseFromArray(data.data(),data.size()))
    {
        qDebug()<<"Parse failed";
    }

    if(message.IsInitialized())
    {
        parse(message);
    }
    else
    {
        qDebug()<<QString::fromStdString(message.InitializationErrorString());
    }
}
