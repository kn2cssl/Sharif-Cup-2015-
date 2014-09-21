#include "tacticconfront.h"

TacticConfront::TacticConfront(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticConfront", worldmodel, parent)
{
    count=0;
    origin=Vector2D(2200,0);
    origin2=Vector2D(-1500,0);
    obs=0;
    rcpast=Vector2D(0,0);
    DoNotEnterOpposedField=false;//true;//
    CanKickOpp = true;

    OppIsValid = true;
}

RobotCommand TacticConfront::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    OppIsValid=wm->theirRobot.IsValid;//wm->ourRobot[8].isValid;
    if(OppIsValid) Opp=wm->theirRobot.position;//wm->ourRobot[8].pos.loc;
    else Opp=Vector2D(100000,100000);
    OppIsInhisField = true;
    rc.fin_pos.loc=origin;
    rc.fin_pos.dir=(origin2-origin).dir().radian();
    rc.maxSpeed = 1.4;
    index=-1;
    int object;

    addData();
    addseg();
    //mergeData();
    sortData();


    //Line2D segline = testseg->line();
    //double y=segline.getX(500);
    //qDebug() << y;

    //    Vector2D proj=testseg->nearestPoint(wm->ball.pos.loc);
    //    qDebug() << proj.x<< "------ Y = " << proj.y ; // length();//atan(perp.a()/perp.b())*180/M_PI;
    //   qDebug() << wm->ball.pos.loc.x<< "------ Y = " << wm->ball.pos.loc.y ;
    //    testseg->existIntersection();

    //---------------------------TRANSFER NEGATIVE OBJECTS OVER THE LINE ---------------
    for(int i=0;i<agentsNegative.size();i++)
    {
        //if(obstacle)wm->kn->)
        //{continue; }
        //origin.x=origin.x*(2*agentsNegative.at(i).goalRegion - 1 );
        o2o = new Segment2D(origin , agentsNegative.at(i) );
        temp=0;
        bool IsOverTheLine=false;
        for(int j=0;j<segList.size(); j++)
        {
            if(segList.at(j).existIntersection(*o2o))
            {
                IsOverTheLine=true;
                break;
            }

        }

        //if(IsOverTheLine) break;

        if(!IsOverTheLine)
        {
            index=i;
            //qDebug() << 2*agentsNegative.at(i).goalRegion - 1 ;
            //qDebug() << agentsNegative.at(i).x << " Y ---- " << agentsNegative.at(i).y;// TOOOOOOOOOOOOOOOOOOOSHE !!!!!!!" << index ;
            //goalRegion=agentsNegative.at(i).goalRegion;
            temp=1;
            break;
        }

    }

    if(index !=-1)
    {
        Vector2D point2 = agentsNegative.at(index);//;//temp.loc;//wm->ourRobot[id].pos.loc;//agentsR0.first().loc;
        Vector2D nrstpnt = findnearest(point2);
        Vector2D diff2 = nrstpnt - point2;// wm->ourRobot[id].pos.loc ;
        bool reach=false;
        // agentsNegative.

        if(temp!=0)
        {
            //            qDebug() << "---------TRANSFER NEGATIVE OBJECTS OVER THE LINE ---------";
            switch(state)
            {
            case 0:{ //Go Behind the Object

                Vector2D space2=diff2;
                //int side = agentsNegative.at(index).goalRegion;
                space2.setLength(300);
                //space2.setDir(space2.dir() + M_PI*side);
                rc.maxSpeed=1.2;
                rc.useNav = true;
                rc.fin_pos.loc=point2  - space2;
                rc.fin_pos.dir=diff2.dir().radian();

                object=findnearestObject(wm->negativeShapes,wm->ourRobot[id].pos.loc);
                if(object!=-1) ObsC=Circle2D(wm->negativeShapes.at(object).position,(wm->negativeShapes.at(object).roundedRadios+ROBOT_RADIUS+250));
                rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);
                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,150);
                if(reach) state = 1;

            }
                break;
            case 1:{//Ready to Push

                rc.useNav = false;
                rc.maxSpeed=1.2;
                int side = -1;//agentsNegative.at(index).goalRegion;
                rc.fin_pos.loc.x=point2.x +(side)*(100+ROBOT_RADIUS)*(diff2.x)/(diff2.length());
                rc.fin_pos.loc.y=point2.y +(side)*(100+ROBOT_RADIUS)*(diff2.y)/(diff2.length());
                rc.fin_pos.dir=diff2.dir().radian();
                if(((wm->ourRobot[id].pos.loc-point2).length())>600)
                {

                    state=0;
                }
                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,150);
                if(reach)
                    state = 2;
            }
                break;
            case 2:{//Push
                //Vector2D diff2 = region2.center() - wm->ourRobot[id].pos.loc ;
                rc.useNav = false;
                rc.maxSpeed=1.1;
                //if(diff2.length() > 1500) diff2.setLength(1500);
                diff2.setLength(300);

                rc.fin_pos.loc=point2 + diff2;//5;
                rc.fin_pos.dir=diff2.dir().radian();

                if(((wm->ourRobot[id].pos.loc-point2).length())>500)
                {
                    state=0;
                }
                if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length())<100)
                {
                    state=0;
                }

                //                Vector2D o2r = ( point2 - wm->ourRobot[id].pos.loc );
                //                if(fabs(wm->ourRobot[id].pos.dir - o2r.dir().radian()) > AngleDeg::deg2rad(30))
                //                {
                //                    qDebug() << " !!!! Out OF Direction !!!! " ;
                //                    state=1;//4;
                //                }

            }
                break;
            case 3:{//Release

                // if(testseg->existIntersection(*o2o))
                //{
                qDebug() << " INNNNNNNNNNNN SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIDE !!!";
                count++;
                //if(index==agentsR1.size()) rc.fin_pos.loc=Vector2D(0,0);
                if(temp==0)
                {
                    rc.fin_pos.loc=origin;//Vector2D(1500,1500);
                    break;
                }
                //agentsR1.takeFirst();
                //index++;
                // }
                //if(reach)
                state = 0;
            }
                break;
            }
        }
    }
    // ---------------------------------------------------------------------------------

    //--------------------------TRANSFER POSITIVE OBJECTS OVER THE LINE ----------------
    if(temp==0)
    {
        //        qDebug() << "---------TRANSFER POSITIVE  ---------";
        for(int i=0;i<agentsPositive.size();i++)
        {
            //if(obstacle)wm->kn->)
            //{continue; }
            //r2o = new Segment2D(Vector2D(wm->ourRobot[id].pos.loc) , Vector2D(agentsPositive.at(i).pos));
            //qDebug() << 2*agentsPositive.at(i).goalRegion - 1 ;
            o2o = new Segment2D(origin2 , Vector2D(agentsPositive.at(i)));
            temp2=0;
            IsOverTheLine=false;
            for(int j=0;j<segList.size(); j++)
            {
                if(segList.at(j).existIntersection(*o2o))
                {
                    IsOverTheLine=true;
                    break;
                }

            }

            if(!IsOverTheLine || agentsPositive.at(i).x < origin.x)
            {
                index=i;
                qDebug() << agentsPositive.at(i).x << " Y ---- " << agentsPositive.at(i).y;// TOOOOOOOOOOOOOOOOOOOSHE !!!!!!!" << index ;
                //goalRegion=agentsPositive.at(i).goalRegion;
                temp2=1;
                break;
            }

        }

        if(index!=-1) // Positive Objects are in field
        {
            Vector2D point2 = agentsPositive.at(index);
            Vector2D nrstpnt = findnearest(point2);
            Vector2D diff2 = nrstpnt - point2;
            bool reach=false;

            if(temp2!=0)
            {
                if(!IsOverTheLine) // transfer that to the nearest point , perpendicular to the line
                {
                    switch(state)
                    {
                    case 0:{ //Go Behind the Object

                        Vector2D space2=diff2;
                        space2.setLength(300);
                        rc.maxSpeed=1.2;
                        rc.useNav = true;
                        rc.fin_pos.loc=point2  - space2;
                        rc.fin_pos.dir=diff2.dir().radian();

                        object=findnearestObject(wm->positiveShapes,wm->ourRobot[id].pos.loc);
                        if(object!=-1) ObsC=Circle2D(wm->positiveShapes.at(object).position,(wm->positiveShapes.at(object).roundedRadios+ROBOT_RADIUS+250));
                        rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

                        reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,50);
                        if(reach) state = 1;

                    }
                        break;
                    case 1:{//Ready to Push

                        rc.useNav = false;
                        rc.maxSpeed=1.1;
                        int side = -1;//agentsPositive.at(index).goalRegion;
                        rc.fin_pos.loc.x=point2.x +(side)*(100 + ROBOT_RADIUS)*(diff2.x)/(diff2.length());
                        rc.fin_pos.loc.y=point2.y +(side)*(100 + ROBOT_RADIUS)*(diff2.y)/(diff2.length());
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
                        if(((wm->ourRobot[id].pos.loc-point2).length())>500) state=0;
                        if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc/*point2*/).length())<50) state=0;
                        diff2.setLength(300);

                        //                        Vector2D o2r = ( point2 - wm->ourRobot[id].pos.loc );
                        //                        if(fabs(wm->ourRobot[id].pos.dir - o2r.dir().radian()) > AngleDeg::deg2rad(20))
                        //                        {
                        //                            qDebug() << " !!!! Out OF Direction !!!! " ;
                        //                            state=1;//4;
                        //                        }

                        rc.fin_pos.loc=point2 + diff2;//5;
                        rc.fin_pos.dir=diff2.dir().radian();
                    }
                        break;
                    case 3:{//Release

                        // if(testseg->existIntersection(*o2o))
                        //{
                        qDebug() << " INNNNNNNNNNNN SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIDE !!!";
                        count++;
                        //if(index==agentsR1.size()) rc.fin_pos.loc=Vector2D(0,0);
                        if(temp2==0)
                        {
                            rc.fin_pos.loc=origin2;//Vector2D(1500,1500);
                            break;
                        }
                        //agentsR1.takeFirst();
                        //index++;
                        // }
                        //if(reach)
                        state = 0;
                    }
                        break;
                    }
                }
                else
                {
                    {
                        switch(state)
                        {
                        case 0:{ //Go Behind the Object

                            Vector2D space2=diff2;
                            //int side = agentsPositive.at(index).goalRegion;
                            space2.setLength(300);
                            //space2.setDir(space2.dir() + M_PI*side);
                            rc.maxSpeed=1.2;
                            rc.useNav = true;
                            rc.fin_pos.loc=point2 ;// - space2;
                            rc.fin_pos.loc.x-=300;
                            rc.fin_pos.dir=0;//diff2.dir().radian();

                            object=findnearestObject(wm->positiveShapes,wm->ourRobot[id].pos.loc);
                            if(object!=-1) ObsC=Circle2D(wm->positiveShapes.at(object).position,(wm->positiveShapes.at(object).roundedRadios+ROBOT_RADIUS+250));
                            rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

                            reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,150);
                            if(reach) state = 1;

                        }
                            break;
                        case 1:{//Ready to Push

                            rc.useNav = false;
                            rc.maxSpeed=1;
                            int side = -1;//agentsPositive.at(index).goalRegion;
                            rc.fin_pos.loc.x=point2.x +(side)*(100+ROBOT_RADIUS);//*(diff2.x)/(diff2.length());
                            rc.fin_pos.loc.y=point2.y;// +(side)*100*(diff2.y)/(diff2.length());
                            rc.fin_pos.dir=0;//diff2.dir().radian();
                            if(((wm->ourRobot[id].pos.loc-point2).length())>600)
                            {

                                state=0;
                            }
                            reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,150);
                            if(reach)
                                state = 2;
                        }
                            break;
                        case 2:{//Push
                            //Vector2D diff2 = region2.center() - wm->ourRobot[id].pos.loc ;
                            rc.useNav = false;
                            rc.maxSpeed=1.3;
                            double delta=((wm->ourRobot[id].pos.loc.x-origin.x)/fabs(wm->ourRobot[id].pos.loc.x-origin.x));
                            delta=delta*300;
                            //if(delta > 1500) delta=1500;


                            rc.fin_pos.loc=point2;
                            rc.fin_pos.loc.x -= delta;
                            rc.fin_pos.dir=0;//diff2.dir().radian();
                            if(((wm->ourRobot[id].pos.loc-point2).length())>500) state=0;
                            if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length())<50) state=0;

                        }
                            break;
                        }
                    }
                }
            }

        }
        //----------------------------------------------------------------------------

    }
    //    qDebug()<< /*"fin_pos.x  " << rc.fin_pos.loc.x << "  Y  "<<rc.fin_pos.loc.y<< " ------------------------------ */"STATE = " << state ;
    //    qDebug() << "USE NAVIGATION IS : " << rc.useNav;

    if(OppIsValid)
    {
        Segment2D Opp2O(Opp , origin2);

        for(int j=0;j<segList.size(); j++)
        {
            if(segList.at(j).existIntersection(Opp2O))
            {
                OppIsInhisField = false;
                break;
            }
        }
    }

    if(OppIsValid && OppIsInhisField && (wm->ourRobot[id].pos.loc-Opp).length() < 1000 ) DoNotEnterOpposedField=true;
    if(DoNotEnterOpposedField)
    {
        // DO NOT ENTER OPPOSED Field
        Segment2D fin2o(origin , rc.fin_pos.loc );

        for(int j=0;j<segList.size(); j++)
        {
            if(segList.at(j).existIntersection(fin2o))
            {
                rc.fin_pos.loc=segList.at(j).nearestPoint(rc.fin_pos.loc);
                qDebug() << "DO NOT ENTER OPPOSED REGION";
                break;
            }
        }
    }

    if(!OppIsInhisField )
    {
        rc.fin_pos.loc = Opp;//wm->ourRobot[8].pos.loc;
    }

    qDebug() << " OPP Is In : (" << Opp.x << "," << Opp.y << " ) , Opp Is Valid Is : " << OppIsValid << " Opp Is In His Field Is : " << OppIsInhisField ;
    rc.fin_pos.loc.x=rcpast.x + 0.6*(rc.fin_pos.loc.x-rcpast.x);
    rc.fin_pos.loc.y=rcpast.y + 0.6*(rc.fin_pos.loc.y-rcpast.y);
    rcpast=rc.fin_pos.loc;
    //    qDebug() << " DIST To Final Pos = " << (wm->ourRobot[id].pos.loc - rc.fin_pos.loc).length();
    rc.maxSpeed /= 1.5;
    rc.useNav=false;
    rc.isBallObs = false;
    rc.isKickObs = true;
    return rc;
}
// ==================================================================================
void TacticConfront::addData()
{

    agentsNegative.clear();
    for(int k=0;k<wm->negativeShapes.size();k++)
    {
        agentsNegative.push_back(wm->negativeShapes.at(k).position);
    }
    //    agentsNegative.insert(0,wm->ball.pos.loc);//wm->ourRobot[0].pos.loc);
    //    agentsNegative.insert(1,wm->ourRobot[1].pos.loc);
    //    agentsNegative.insert(2,wm->ourRobot[2].pos.loc);

    agentsPositive.clear();
    for(int k=0;k<wm->positiveShapes.size();k++)
    {
        agentsPositive.push_back(wm->positiveShapes.at(k).position);
    }
    //    agentsPositive.insert(0,wm->ourRobot[4].pos.loc);//wm->ourRobot[0].pos.loc);
    //    agentsPositive.insert(1,wm->ourRobot[5].pos.loc);

    //    agentsPositive.insert(2,wm->oppRobot[0].pos.loc);
}
// ==================================================================================
void TacticConfront::addseg()
{

    segList.clear(); // SO IMPORTANT !!!

    //tseg = new Segment2D(Vector2D(-500, 2000),Vector2D(0,0));
    //segList.insert(0,*tseg);
    //tseg = new Segment2D(Vector2D(-500,-2000),Vector2D(0,0));
    //segList.insert(1,*tseg);
    segList=wm->borders;

}
// ==================================================================================
Vector2D TacticConfront::AvoidtoEnterCircle(Circle2D Ci, Vector2D pnt, Vector2D finPOS)
{
    //    Avoided=false;
    Segment2D S(pnt,finPOS);
    if(Ci.HasIntersection(S) || Ci.contains(pnt) || Ci.contains(finPOS))
    {
        //        qDebug() << " HAS INTERSECTION WITH CIRCLE !! DO NOT ENTER THIS CIRCLE PLEASE !!!";
        //        qDebug() << " HAS INTERSECTION WITH CIRCLE ( " << Ci.center().x << "," << Ci.center().y << " ) , Radius : " << Ci.radius() ;
        Vector2D c2r = pnt - Ci.center() ;
        Vector2D c2f = finPOS - Ci.center() ;
        c2r.setLength(Ci.radius()+50);
        AngleDeg angle=c2r.dir() - c2f.dir();
        //double ang=c2r.dir().radian() - c2f.dir().radian();
        if (angle.radian() > M_PI) angle -=  AngleDeg(M_PI) *= 2;
        if (angle.radian() < -M_PI) angle += AngleDeg(M_PI) *= 2;

        //if (ang > M_PI) ang -= 2 * M_PI;
        //if (ang < -M_PI) ang += 2 * M_PI;

        //qDebug() << "c2r DIRECTION = " << c2r.dir().degree() << "c2f DIRECTION = " << c2f.dir().degree() << "   ANGLE  = " << angle.degree() << "    ANG =" << ang*57.32;
        int div=int(Ci.radius()/50);
        //        qDebug() << "div = " << div ;
        angle=angle/=min(div,6);
        angle=c2r.dir() - angle;
        c2r.setDir(angle);// c2f.dir()));
        Vector2D p = Ci.center() + c2r ;
        //        Avoided=true;
//        qDebug() << " Avoided point is : (" << finPOS.x << "," <<finPOS.y << ")  & Corrected is : ("<<
//                    p.x << "," << p.y << ")" ;
        return p;
    }
    else
    {
        return finPOS;
    }
}
// ==================================================================================
Vector2D TacticConfront::findnearest(Vector2D input)
{
    //int tmp = 0 ;
    double dist;
    Vector2D point=Vector2D(1000000,1000000);//,point2;
    for(int k=0;k<segList.size();k++)
    {
        dist=(input-point).length2();
        if((input-segList.at(k).nearestPoint(input)).length2()<dist) point=segList.at(k).nearestPoint(input);

    }
    return point;
}
// ==================================================================================
int TacticConfront::findnearestObject(QList<Shape> list, Vector2D Pos)
{
    double dist=1000000000000;
    int indX=-1;
//    qDebug() << " yooooooooooohoooooooooooo";
    for(int i=0;i<list.size();i++)
    {
//        qDebug() << " yooooooooooohoooooooooooo";
        double dist2= (Pos-list.at(i).position).length2();
        if(dist2<dist)
        {
            dist=dist2;
            indX=i;

        }
    }
    return indX;
}

//void TacticConfront::mergeData()
//{
//    mergedList.clear();
//    for(int i=0;i<agentsR0.size();i++)
//    {
//        AgentsAndRegions temp;
//        temp.pos=agentsR0.at(i);
//        temp.goalRegion=0;
//        mergedList.insert(i,temp);
//    }

//    for(int i=0;i<agentsR1.size();i++)
//    {
//        AgentsAndRegions temp;
//        temp.pos=agentsR1.at(i);
//        temp.goalRegion=1;
//        mergedList.insert(i+agentsR0.size(),temp);
//    }
//}
// ==================================================================================
void TacticConfront::sortData()
{
    for(int i=0;i<agentsPositive.size();i++)
    {
        //for(int j=0;j<segList.size();j++)
        //{
        for(int k=i+1;k<agentsPositive.size();k++)
        {
            if(( (agentsPositive.at(i)-wm->ourRobot[id].pos.loc).length2() +(agentsPositive.at(i)-findnearest(agentsPositive.at(i))).length2())
                    > ((agentsPositive.at(k)-wm->ourRobot[id].pos.loc).length2() +((agentsPositive.at(i)- findnearest(agentsPositive.at(k))).length2() )) ) agentsPositive.swap(i,k);
        }
        //}

    }


    for(int i=0;i<agentsNegative.size();i++)
    {
        //for(int j=0;j<segList.size();j++)
        //{
        for(int k=i+1;k<agentsNegative.size();k++)
        {
//            if( (agentsNegative.at(i)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(i))*/).length2()
//                    > (agentsNegative.at(k)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(k))*/).length2() ) agentsNegative.swap(i,k);

            if(( (agentsNegative.at(i)-wm->ourRobot[id].pos.loc).length2() +(agentsNegative.at(i)-findnearest(agentsNegative.at(i))).length2())
                    > ((agentsNegative.at(k)-wm->ourRobot[id].pos.loc).length2() +((agentsNegative.at(i)- findnearest(agentsNegative.at(k))).length2() )) ) agentsNegative.swap(i,k);


        }
        //}

    }
}
// ==================================================================================
// ==================================================================================
