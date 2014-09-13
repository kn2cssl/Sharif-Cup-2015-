#include "tactictest.h"


TacticTest::TacticTest(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticTest", worldmodel, parent)
{
    circularBorder.assign(Vector2D(1500,0),1700/2);
    circularBorderOut.assign(Vector2D(1500,0),2100/2);// a circle may use to push balls with some risks
    circularMid.assign(Vector2D(1500,0),720); // a circle which is between holes and border circle
    hole1.assign(Vector2D(1500,1700/4),250);
    hole2.assign(Vector2D(1500,-1700/4),250);
    Vector2D Cdist = (hole1.center() - circularBorder.center());
    double deltaAngle=1.1*asin(hole1.radius()/(Cdist.length())); // 1.1 is safety factor
    Uangle1=Cdist.dir().radian() + deltaAngle ;
    Dangle1=Uangle1 - 2*deltaAngle;
    Cdist = (hole2.center() - circularBorder.center());
    Uangle2=Cdist.dir().radian() + deltaAngle ;
    Dangle2=Uangle2 - 2*deltaAngle;
    state=0;

}

RobotCommand TacticTest::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    switch(state)
    {
    case 0: {
        rc.fin_pos.loc=Vector2D(1000,1000);
        if(wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,50)) state=1;
    }
        break;
    case 1: {
        rc.fin_pos.loc=Vector2D(2000,1000);
        if(wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,50)) state=2;
    }
        break;
    case 2: {
        rc.fin_pos.loc=Vector2D(1000,-1000);
        if(wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,50)) state=3;
    }
        break;
    case 3: {
        rc.fin_pos.loc=Vector2D(2000,-1000);
        if(wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,50)) state=0;
    }
        break;
    }

    rc.maxSpeed = 1.1;
    rc.isBallObs = false;
    rc.isKickObs = true;
    return rc;
}
// ==================================================================================
void TacticTest::addData()
{

    balls.clear();
    balls.insert(0,&wm->ball);//wm->ourRobot[0].pos.loc);

    //balls.insert(1,&wm->ourRobot[1]);
    //balls.insert(2,wm->ourRobot[2].pos.loc);

    oppIsInField = wm->oppRobot[4].isValid;
    OppositeRobot = wm->oppRobot[4].pos.loc;
}

// ==================================================================================
//Vector2D TacticTest::findnearest(Vector2D input)
//{
//    //int tmp = 0 ;
//    double dist;
//    Vector2D point=Vector2D(1000000,1000000);//,point2;
//    for(int k=0;k<segList.size();k++)
//    {
//        dist=(input-point).length2();
//        if((input-segList.at(k).nearestPoint(input)).length2()<dist) point=segList.at(k).nearestPoint(input);

//    }
//    return point;
//}


// ==================================================================================
void TacticTest::sortData()
{
    for(int i=0;i<balls.size();i++)
    {
        //for(int j=0;j<segList.size();j++)
        //{
        for(int k=i+1;k<balls.size();k++)
        {
            if( (balls.at(i)->pos.loc-circularBorder.center()).length2()
                    > (balls.at(k)->pos.loc-circularBorder.center()).length2() ) balls.swap(i,k);
        }
        //}

    }

}
// ==================================================================================
void TacticTest::goOncircle2point(Vector2D pnt)
{

}

// =================================================================================
Vector2D TacticTest::findnearest2circle()
{
    int tmp=0;
    for(int k=0;k<balls.size();k++)
    {
        if( (balls.at(k)->pos.loc-circularBorder.center()).length2() < (balls.at(tmp)->pos.loc-circularBorder.center()).length2()) tmp=k;
    }
    return balls.at(tmp)->pos.loc;
}
// ==================================================================================
Vector2D TacticTest::findnearesthole(Vector2D pnt)
{
    if( (pnt-hole1.center()).length2() < (pnt-hole2.center()).length2() ) return hole1.center();
    else return hole2.center();
}
// ==================================================================================
// ==================================================================================
