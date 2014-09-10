#include "sslvision.h"

SSLVision::SSLVision(QString ip, int port, TeamColorType color, TeamSideType side, CameraConfigType camera, WorldModel *wm, QObject *parent) :
    SSLReceiver(ip,port,parent),
    _color(color),
    _side(side),
    _camera(camera),
    _fpscam0(parent),
    _fpscam1(parent),
    _wm(wm)
{
    _time.start();
    connect(this, SIGNAL(newReceivedPacket(QByteArray,QString,int)), this, SLOT(readPendingPacket(QByteArray,QString,int)));

    // Log
    qDebug() << "SSLVision Initialization...";
    qDebug() << "IP: " << ip;
    qDebug() << "Port: " << port;
    qDebug() << "Color: " << (color==COLOR_BLUE?"Blue":"Yellow");
    qDebug() << "Side: " << (side==SIDE_RIGHT?"Right":"Left");
    qDebug() << "Camera: " << ((int)camera);

    //firstTimeInitialize = false;
}

int SSLVision::getFPS(int c)
{
    if(c == 0) return _fpscam0.FPS();
    if(c == 1) return _fpscam1.FPS();
    return 0;
}

void SSLVision::readPendingPacket(QByteArray data, QString ip, int port)
{
    // check for server ip (& port)
    if(ip=="" && port==0) return;

    // unpack sslvision packet
    SSL_WrapperPacket packet;
    bool ans=packet.ParseFromArray(data.data(), data.size());
    if(!ans) return;
    if(packet.has_detection()==false) return;
    SSL_DetectionFrame pck = packet.detection();

    // parse detection frame
    parse(pck);
}

void SSLVision::parse(SSL_DetectionFrame &pck)
{

    // update camera fps
    int cid = pck.camera_id();
    if(cid == 0) _fpscam0.Pulse();
    if(cid == 1) _fpscam1.Pulse();

    switch(_camera)
    {
    case CAMERA_BOTH:
        break;
    case CAMERA_0:
        if (cid==1) return;
        break;
    case CAMERA_1:
        if (cid==0) return;
        break;
    case CAMERA_NONE:
    default:
        return;
    }

    // update vision frame
    //_vframe[cid].frame_number =  pck.frame_number();

    vector<Position> pt;

    // Team side Coefficient
    float ourSide = (_side == SIDE_RIGHT)? -1.0f : 1.0f;
    double time = _time.elapsed(); //pck.t_capture();

    // insert balls
    int max_balls=min(VOBJ_MAX_NUM, pck.balls_size());
    //_wm->ballsWithoutSpeed.clear();
    for(int i=0; i<max_balls; ++i)
    {
        auto b = pck.balls(i);
//        if(b.has_confidence() && b.has_x() && b.has_y())
//            if(b.confidence() > MIN_CONF && fabs(b.x()) < FIELD_MAX_X && fabs(b.y()) < FIELD_MAX_Y)
//            {
//                Position tp;
//                tp.loc = Vector2D(b.x()*ourSide, b.y()*ourSide);
//                pt.push_back(tp);
//            }
        // gand started
        if(fabs(b.x()) < FIELD_MAX_X && fabs(b.y()) < FIELD_MAX_Y)
        {
            Position tp;
            tp.loc = Vector2D(b.x()*ourSide, b.y()*ourSide);
            pt.push_back(tp);
//            _wm->ballsWithoutSpeed.push_back(tp);
//            qDebug()<<"ball"<<i<<":"<<tp.loc.x<<","<<tp.loc.y;
        }

        // end gand

    }
    _wm->ball.seenAt(pt, time, cid);

    //_wm->numberOfBalls = pt.size();

    QList<Ball*> ballList;

    for(int i=0;i<pt.size();i++)
    {
        Ball *tBall = new Ball();
        tBall->pos = pt.at(i);
        ballList.push_back(tBall);
        //delete tBall;
    }
     vector<Position> list;

    for(int i=0;i<ballList.size();i++)
    {
        list.clear();

        int index = findNearestBall(ballList.at(i),good);

        if(index == -1)
        {
            index = findNearestBall(ballList.at(i),candidate);

            if(index == -1)
            {
                candidate.push_back(ballList.at(i));
//                list.push_back(ballList.at(i)->pos);
                candidate.last()->checked = true;
//                candidate.last()->seenAt(list, time, cid);
            }
            else
            {
                candidate.at(index)->frameCount++;
                candidate.at(index)->checked = true;
//                list.push_back(ballList.at(i)->pos);
//                candidate.at(index)->seenAt(list, time, cid);
            }
        }
        else
        {
            //good.at(index)->frameCount++;
            good.at(index)->checked = true;
            list.push_back(ballList.at(i)->pos);
            good.at(index)->seenAt(list, time, cid);
        }
    }

    for(int i=0;i<candidate.size();i++)
    {
        if(candidate.at(i)->checked)
        {
            candidate.at(i)->checked = false;
            if( candidate.at(i)->frameCount > 20)
            {
                good.push_back(candidate.at(i));
                good.last()->frameCount = 40;
                candidate.removeAt(i);
            }
        }
        else
        {
            candidate.at(i)->checked = false;
            candidate.at(i)->frameCount--;
            if( candidate.at(i)->frameCount <= 0)
            {
                candidate.removeAt(i);
            }
        }
    }

    for(int i=0;i<good.size();i++)
    {
        if(good.at(i)->checked)
        {
            good.at(i)->checked = false;
        }
        else
        {
            good.at(i)->checked = false;
            good.at(i)->frameCount--;
            if( good.at(i)->frameCount < 20)
            {
                candidate.push_back(good.at(i));
                good.removeAt(i);
            }
        }
    }

    _wm->balls.clear();
    for(int i=0;i<good.size();i++)
    {
        _wm->balls.push_back(good.at(i));
//        qDebug() << " Frame Count = " << good.at(i)->frameCount ;
    }

//    qDebug()<<"scanned";

    if(_color == COLOR_BLUE)
    {
        APPEND_ROBOTS(blue, our);
        APPEND_ROBOTS(yellow, opp);
    }
    else // _color == COLOR_YELLOW
    {
        APPEND_ROBOTS(yellow, our);
        APPEND_ROBOTS(blue, opp);
    }

}

int SSLVision::findNearestBall(Ball *ball, QList<Ball *> input_balls)
{
    double dist = 1000000000000000;
    int index = -1;
    for(int i=0;i<input_balls.size();i++)
    {
        if(!input_balls.at(i)->checked)
        {
            double dist2 = ball->pos.loc.dist2(input_balls.at(i)->pos.loc);
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
    return index;
}
