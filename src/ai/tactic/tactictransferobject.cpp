#include "tactictransferobject.h"

TacticTransferObject::TacticTransferObject(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticTransferObject", worldmodel, parent)
{

    region[0].assign(Vector2D(-300,-1500),Size2D(1000,1000));
//    region[0].assign(Vector2D(1000,-500),Size2D(1000,1000));
    region[1].assign(Vector2D(2000,300),Size2D(1000,1000));
    state=0;
    rcpast=Vector2D(0,0);
    //index=0;
    firstInit=true;//false;

    MAX_X = 2200; MIN_X=200;
    MAX_Y = 1300; MIN_Y=-1400;//750;
    mean_x=(MAX_X+MIN_X) / 2;
    mean_y=(MAX_Y+MIN_Y) / 2;

}

RobotCommand TacticTransferObject::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    rc.useNav=true;
    rc.maxSpeed = 1;
    rc.fin_pos.loc=Vector2D(300,0);
    //    qDebug() << " TOP LEFT.x = " <<region[0].topLeft().x << "    TOP LEFT . Y =  " << region[0].topLeft().y;
    //    qDebug() << " BOTTOM RIGHT.x = " <<region[0].bottomRight().x << "    BOTTOM ROGHT . Y =  " << region[0].bottomRight().y;
    addData();
    mergeData();
//    qDebug()<< " IIIIIIIIIIIIIIIIIIIIINNNNNNNNNNNNNN JAAAAA  !!!!!!!";
    sortData();

//    for(int j=0;j<mergedList.size();j++)


    //qDebug()<< " BALLL . X = " << wm->ball.pos.loc.x << " BALLL . Y = " <<  wm->ball.pos.loc.y;
    //if(region[0].IsInside(wm->ball.pos.loc)) qDebug() << " THE BALLLLL ISSS INNNNN SIDE !!!!!!!!!!!!!!!!!!!!!!1";

    index = -1;
    for(int i=0;i<mergedList.size();i++)
    {
        temp=0;
        if(!region[mergedList.at(i).goalRegion].IsInside(mergedList.at(i).pos))
        {
            //qDebug() <<" OBJECT :    " <<  mergedList.at(i).pos.x << " ------ Y = " << mergedList.at(i).pos.y;// TOOOOOOOOOOOOOOOOOOOSHE !!!!!!!" << index ;
            index=i;
            goalRegion=mergedList.at(i).goalRegion;
            temp=1;
            break;
        }


    }





    if(index != -1)
    {
        Vector2D point2 = mergedList.at(index).pos;//;//temp.loc;//wm->ourRobot[id].pos.loc;//agentsR0.first().loc;
        Vector2D diff2 = region[goalRegion].center() - point2;// wm->ourRobot[id].pos.loc ;
        bool reach=false;


        if(temp!=0)
        {
            switch(state)
            {
            case 0:{ //Go Behind the Object

                Vector2D space2=diff2;
                space2.setLength(300);
                rc.maxSpeed=1.4;
                rc.useNav = true;
                rc.fin_pos.loc=point2 - space2;
                rc.fin_pos.dir=diff2.dir().radian();
                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,150);
                if(reach) state = 1;

            }
                break;
            case 1:{//Ready to Push
                rc.useNav = false;
                rc.maxSpeed=1.2;
                rc.fin_pos.loc.x=point2.x - (100 + ROBOT_RADIUS)*(diff2.x)/(diff2.length());
                rc.fin_pos.loc.y=point2.y - (100 + ROBOT_RADIUS)*(diff2.y)/(diff2.length());
                rc.fin_pos.dir=diff2.dir().radian();
                if(((wm->ourRobot[id].pos.loc-point2).length())>400) state=0;
                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,100);
                if(reach)
                    state = 2;
            }
                break;
            case 2:{//Push
                //Vector2D diff2 = region2.center() - wm->ourRobot[id].pos.loc ;
                rc.useNav = false;
                rc.maxSpeed=1;
                //if(diff2.length() > 1500) diff2.setLength(1500);
                // if(((wm->ourRobot[id].pos.loc-point2).length())>400) state=0;
                diff2.setLength(200);
                if(((wm->ourRobot[id].pos.loc-point2).length())>500) state=0;
                if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length())<50) state=0;
                Vector2D o2r = ( point2 - wm->ourRobot[id].pos.loc );
                if(fabs(wm->ourRobot[id].pos.dir - o2r.dir().radian()) > AngleDeg::deg2rad(40))
                {
                    qDebug() << " !!!! Out OF Direction !!!! " ;
                    state=1;//4;
                }
                rc.fin_pos.loc=point2 + diff2;//5;
                rc.fin_pos.dir=diff2.dir().radian();
                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,150);
                if(reach)
                    state = 3;
            }
                break;
            case 3:{//Release

                if(region[goalRegion].IsInside(point2))
                {
                    //qDebug() << " INNNNNNNNNNNN SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIDE !!!";
                    //if(index==agentsR1.size()) rc.fin_pos.loc=Vector2D(0,0);
                    if(temp==0)
                    {
                        rc.fin_pos.loc=Vector2D(0,0);
                        break;
                    }
                    //agentsR1.takeFirst();
                    //index++;
                }
                //if(reach)
                state = 0;
            }
                break;

            case 4:{// back behind the object


            }
                break;
            }
        }


        //qDebug() << rc.fin_pos.loc.x << " -------  Y = " << rc.fin_pos.loc.y << " STATE = " << state;
        qDebug() << "STATE = " << state;
    }

    //rc.fin_pos.loc=Vector2D(1300,0);
//    bool reachZ=wm->kn->ReachedToPos(wm->ourRobot[id].pos,rc.fin_pos,40,5);
//    if(reachZ) rc.fin_pos.loc=wm->ourRobot[id].pos.loc;
//    qDebug() << " USE NAVIGATION IS :   " << rc.useNav;
    //rc.maxSpeed = 1.2;//rc.maxSpeed;

//    rc.fin_pos.loc.x=rcpast.x + 0.1*(rc.fin_pos.loc.x-rcpast.x);
//    rc.fin_pos.loc.y=rcpast.y + 0.1*(rc.fin_pos.loc.y-rcpast.y);

//    rcpast=rc.fin_pos.loc;

    rc.maxSpeed/=1.4;
//    rc.fin_pos.loc=KeepInField(rc);

    qDebug() << " This Object Is For Region " << goalRegion ;

    rc.isBallObs = false;
    rc.isKickObs = true;
    return rc;
}

void TacticTransferObject::addData()
{
    //agentsR1.clear();
    //agentsR1.insert(0,wm->ball.pos.loc);//wm->ourRobot[0].pos.loc);

    //agentsR1.insert(1,wm->ourRobot[1].pos.loc);
    //agentsR1.insert(2,wm->ourRobot[2].pos.loc);
    //agentsR1=wm->shapes4Region1;

    //    agentsR0.clear();
    //    for ( int i=0;i < wm->shapes4Region1.size();i++)
    //    {
    //    agentsR0.insert(i,wm->shapes4Region1.at(i).position);
    //    }
    //agentsR0.insert(0,wm->ourRobot[4].pos.loc);//wm->ourRobot[0].pos.loc);
    //agentsR0.insert(1,wm->ourRobot[5].pos.loc);

    //agentsR0.insert(2,wm->oppRobot[0].pos.loc);

}

void TacticTransferObject::mergeData()
{
    mergedList.clear();
    for(int i=0;i<wm->shapes4Region1.size();i++)//agentsR0.size();i++)
    {
        AgentsAndRegions temp;
        temp.pos=wm->shapes4Region1.at(i).position;
        temp.goalRegion=0;
        mergedList.push_back(temp);
    }

        for(int i=0;i<wm->shapes4Region2.size();i++)
        {
            AgentsAndRegions temp;
            temp.pos=wm->shapes4Region2.at(i).position;
            temp.goalRegion=1;
            mergedList.push_back(temp);
        }
}

void TacticTransferObject::sortData()
{
    for(int i=0;i<mergedList.size();i++)
    {
        for(int k=i+1;k<mergedList.size();k++)
        {
            if( (mergedList.at(i).pos-region[mergedList.at(i).goalRegion].center()).length2()
              > (mergedList.at(k).pos-region[mergedList.at(k).goalRegion].center()).length2() ) mergedList.swap(i,k);
        }

    }
}

// ==================================================================================

Vector2D TacticTransferObject::KeepInField(RobotCommand rc)
{
    if(rc.fin_pos.loc.x > MAX_X)
    {
        rc.fin_pos.loc = Vector2D(MAX_X,rc.fin_pos.loc.y);
        qDebug() << " X > MAX_X !! ";
    }
    else if(rc.fin_pos.loc.x < MIN_X)
    {
        rc.fin_pos.loc = Vector2D(MIN_X,rc.fin_pos.loc.y);
        qDebug() << " X < MIN_X !! ";
    }

    if(rc.fin_pos.loc.y > MAX_Y)
    {
        rc.fin_pos.loc = Vector2D(rc.fin_pos.loc.x,MAX_Y);
        qDebug() << " Y > MAX_Y !! ";
    }
    else if(rc.fin_pos.loc.y < MIN_Y)
    {
        rc.fin_pos.loc = Vector2D(rc.fin_pos.loc.x,MIN_Y);
        qDebug() << " Y < MIN_Y !! ";
    }

    return rc.fin_pos.loc;

}

// ==================================================================================


