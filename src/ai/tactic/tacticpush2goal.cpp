#include "tacticpush2goal.h"


TacticPush2Goal::TacticPush2Goal(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticPush2Goal", worldmodel, parent)
{


    //    hole1.assign(Vector2D(1500+167,110+(1700/4)),250/2);//250
    //    hole2.assign(Vector2D(1500+167,110-(1700/4)),250/2);
    //    hole1_Offset.assign(Vector2D(1500+167,110+(1700/4)),300/2);//300
    //    hole2_Offset.assign(Vector2D(1500+167,110-(1700/4)),300/2);

    DontEnterCircle=true;

    Vector2D SafePoint=Vector2D(500,0);

    MAX_X = 3500; MIN_X=0;
    MAX_Y = 1750; MIN_Y=-1750;//750;

    //    MAX_X = wm->circularBorder.center().x+3500/2; MIN_X=wm->circularBorder.center().x-3500/2;
    //    MAX_Y = wm->circularBorder.center().y+3500/2; MIN_Y=wm->circularBorder.center().y-3500/2;


    mean_x=(MAX_X+MIN_X) / 2;
    mean_y=(MAX_Y+MIN_Y) / 2;


    statemargin=0;
}

RobotCommand TacticPush2Goal::getCommand()
{
    oppIsValid = false;//wm->ourRobot[0].isValid;//wm->theirRobot.IsValid;// opposite robot

    addData();

    sortData();

    if(!oppIsValid) opp = Vector2D(1000000,1000000);
    //opp = wm->ourRobot[0].pos.loc;//wm->theirRobot.position;//wm->ourRobot[8].pos.loc;
    OppIsKhoraak=false;//(!circularBorder2.contains(opp));//out of his field
    bool reach=false;
    Avoided=false;
    AvoidCommand=false;
    AllIn=true;
    AnyIn=false;
    AllInCorner=false;
    AllUnAccessible=true;

    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    rc.fin_pos.loc=Vector2D(500,0);//circularBorder.center();
    rc.maxSpeed = 1.2;


    index=-1;
    int tah=balls.size()-1;
    if(tah != -1)
    {
        rc.fin_pos.loc=circularBorder2.nearestpoint(balls.at(tah)->pos.loc);
        rc.fin_pos.dir=(circularBorder.center()-rc.fin_pos.loc).dir().radian();
    }


    FindBall(); // find approtiate ball

    //    if( index != -1 )
    //    {
    //        point2 = balls.at(index)->pos.loc;

    //        FindHole();

    //        Vector2D nrstpnt = (circularBorder.center()-point2); //nearest Point
    //        nrstpnt=nrstpnt.setLength(nrstpnt.length()-circularBorder2.radius());
    //        diff2 = nrstpnt;
    //        nrstpnt=point2+nrstpnt;
    //        //        state2=0;
    //        switch(state)
    //        {
    //        case 0:{ //Go Behind the Object

    //            vec2goal.setLength(250);
    //            // //**//**qDebug()<<"VEC 2 GOAL LENGTH = " << vec2goal.length();
    //            rc.maxSpeed=1.3;
    //            rc.useNav = false;//true;
    //            rc.isBallObs = false;//true;
    //            rc.isKickObs = false;//true;
    //            rc.fin_pos.loc=wm->kn->PredictDestination(wm->ourRobot[id].pos.loc,point2  - vec2goal,
    //                                                      rc.maxSpeed,balls.at(index)->vel.loc);//point2  - vec2goal;
    //            //            wm->kn->PredictDestination()
    //            int rad = 150+ROBOT_RADIUS;
    //            Circle2D c(point2,rad);

    //            rc.fin_pos.loc=AvoidtoEnterCircle(c,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);
    //            //            if(Avoided) rc.maxSpeed=rc.maxSpeed;
    //            rc.fin_pos.dir=vec2goal.dir().radian();
    ////            KeepInField(rc);
    //            reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,100);
    //            if(reach && !Avoided) state = 1;

    //        }
    //            break;
    //        case 1:{//Push

    //            rc.useNav = false;
    //            rc.isBallObs = false;
    //            rc.isKickObs = false;
    //            rc.maxSpeed=1.1;
    //            vec2goal.setLength(100);
    //            rc.fin_pos.loc=wm->kn->PredictDestination(wm->ourRobot[id].pos.loc,point2  + vec2goal,
    //                                                      rc.maxSpeed,balls.at(index)->vel.loc);
    //            rc.fin_pos.dir=vec2goal.dir().radian();

    //            KeepInField(rc);
    //            if(((wm->ourRobot[id].pos.loc-point2).length())>800) state=0;
    //            if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length())<250) state=0;


    //            if(hole1.contains(rc.fin_pos.loc) || hole2.contains(rc.fin_pos.loc))
    //            {
    //                //                vec2goal.setLength(100);
    //                rc.fin_pos.loc=point2  ;//+ vec2goal;
    //                //                rc.fin_pos.loc=point2;
    //            }

    //            //            reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,40);
    //            //            if(reach)
    //            //                state2 = 0;
    //        }
    //            break;
    //        }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //shahin







    if( index == -1)//#100
    {
        rc.fin_pos.loc = Vector2D (500,0);
    }

    else
    {
        point2 = balls.at(index)->pos.loc;
        Vector2D r2center=circularBorder.center()-wm->ourRobot[id].pos.loc;

        //Circle2D C2(point2,100);

        //vec2goal=circularBorder.center() - C2.center();

        FindHole();

        if(temp2!=0)//out of the circleborderout
        {

            switch(state)
            {
            case 0:{ //Go Behind the Object


                vec2goal.setLength(250);
                // //**//**qDebug()<<"VEC 2 GOAL LENGTH = " << vec2goal.length();
                rc.maxSpeed=0.8;
                rc.useNav = false;//true;
                rc.isBallObs = false;//true;
                rc.isKickObs = false;//true;
                rc.fin_pos.loc=point2  - vec2goal;

                int rad = 200+ROBOT_RADIUS;
                Circle2D c(point2,rad);

                rc.fin_pos.loc=AvoidtoEnterCircle(c,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);
                //            if(Avoided) rc.maxSpeed=rc.maxSpeed;
                rc.fin_pos.dir=(point2-wm->ourRobot[id].pos.loc).dir().radian();//vec2goal.dir().radian(); //////////////////////// CHANGED !!!

                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,150);
                if(reach) state = 1;

            }
                break;

            case 1:{//push the ball to the realising point(the frontier of the circularborder out)

                Vector2D nearstpoint=circularBorder.nearestpoint(point2);//wm->ourRobot[id].pos.loc);//?????????? ball
                vec2goal.setLength(ROBOT_RADIUS+50);
                rc.fin_pos.loc=nearstpoint-vec2goal;
                rc.fin_pos.dir=vec2goal.dir().radian();//(point2-wm->ourRobot[id].pos.loc).dir().radian();// //////////////////////// CHANGED !!!
                rc.maxSpeed=0.8;




                if(((wm->ourRobot[id].pos.loc-point2).length())>200) state=0;
                if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length())<30) state=0;




                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,50);
                if(reach && !Avoided) state = 0;

            }

                break;

            }

        }

        else
        {


            //qDebug()<<"Shahram";
            switch(state2)
            {
            case 0:{ //Go Behind the Object


                vec2goal.setLength(300);
                // //**//**qDebug()<<"VEC 2 GOAL LENGTH = " << vec2goal.length();
                rc.maxSpeed=0.8;
                rc.useNav = false;//true;
                rc.isBallObs = false;//true;
                rc.isKickObs = false;//true;
                rc.fin_pos.loc=point2  - vec2goal;

                int rad = 200+ROBOT_RADIUS;
                Circle2D c(point2,rad);

                rc.fin_pos.loc=AvoidtoEnterCircle(c,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);
                //            if(Avoided) rc.maxSpeed=rc.maxSpeed;
                rc.fin_pos.dir=(point2-wm->ourRobot[id].pos.loc).dir().radian();

                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,150);
                if(reach && !Avoided) state2 = 1;

            }
                break;


            case 1:{//shoot


                rc.fin_pos.dir=vec2goal.dir().radian();//(point2-wm->ourRobot[id].pos.loc).dir().radian();// //////////////////////// CHANGED !!!
                vec2goal.setLength(100);
                rc.fin_pos.loc=point2 + vec2goal;
                rc.maxSpeed=0.8;

                if(((wm->ourRobot[id].pos.loc-point2).length())>400) state2=0;

            }

                break;
            }


        }


    }




    //shahout

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int mrgn=300;
    Vector2D dlta;
    if(IsInmargins(point2,mrgn))
    {
        qDebug() << "IS IN MARGIN ";
        int side = ((point2.x-mean_x)/abs(point2.x-mean_x))*((point2.y-mean_y)/abs(point2.y-mean_y));
        if(point2.x > MAX_X-mrgn || point2.x < MIN_X+mrgn) {
            side *= ((point2.y-mean_y)/abs(point2.y-mean_y));
            dlta=Vector2D(0,side*(ROBOT_RADIUS+20));}
        else if(point2.y > MAX_Y-mrgn || point2.y < MIN_Y+mrgn) {
            side *=((point2.x-mean_x)/abs(point2.x-mean_x));
            dlta=Vector2D(side*(ROBOT_RADIUS+20),0);}
        switch(statemargin)
        {
        case 0:{
            if(point2.x > MAX_X-mrgn)
                dlta.x=-70;
            else if(point2.x < MIN_X+mrgn)
                dlta.x=+70;
            if(point2.y > MAX_Y-mrgn)
                dlta.y=-70;
            else if(point2.y < MIN_Y+mrgn)
                dlta.y=+70;

            rc.fin_pos.loc=point2+dlta;

            int rad = 70+ROBOT_RADIUS;
            Circle2D c(point2,rad);
            rc.fin_pos.loc=AvoidtoEnterCircle(c,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

            //**//**qDebug()<< "In Margins Pos  : ball = ( " << point2.x << ","<< point2.y << ")";
            //**//**qDebug()<< "In Margins Pos  : delta = ( " << dlta.x << ","<< dlta.y << ")";
            //**//**qDebug()<< "In Margins Pos  : fin_pos = ( " << rc.fin_pos.loc.x << ","<<rc.fin_pos.loc.y << ")";
            //**//**qDebug()<< "In Margins Pos  : Robot = ( " << wm->ourRobot[id].pos.loc.x << ","<<wm->ourRobot[id].pos.loc.y << ")";
            rc.fin_pos.dir=dlta.dir().radian()-side*M_PI/2;
            reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos,rc.fin_pos,20,7);
            //                            wm->ourRobot[id].pos.loc,rc.fin_pos.loc,200);
            //**//**qDebug() << "dist To final Pos : " << (wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length();
            //**//**qDebug() << " Avoided : " << Avoided << "     reach" << reach;
            if(reach) statemargin = 1;
        }
            break;

        case 1:{
            rc.fin_pos.dir = dlta.dir().radian() - side*0.9*M_PI ;
            rc.fin_pos.loc=point2-dlta;
            //**//**qDebug() << "Fin_POS . dir = " << AngleDeg::rad2deg(rc.fin_pos.dir) << " ROBOT . dir = " <<  AngleDeg::rad2deg(wm->ourRobot[id].pos.dir);
            if(((wm->ourRobot[id].pos.loc-point2).length())>300) statemargin=0;
            double delta_ang=wm->ourRobot[id].pos.dir-rc.fin_pos.dir;
            if (delta_ang >  M_PI) delta_ang -= (M_PI * 2);
            if (delta_ang < -M_PI) delta_ang += (M_PI * 2);
            if(fabs(delta_ang) < AngleDeg::deg2rad(10)) statemargin=0;
            rc.maxSpeed=1.7;
            //                bool chighz=wm->kn->ReachedToPos(wm->ourRobot[id].pos,rc.fin_pos,20,10);
            //                if(chighz) statemargin=0;
            //                if((wm->ourRobot[id].pos.loc.dir()-rc.fin_pos.dir)<AngleDeg::deg2rad(10)) statemargin=0;
            //                if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length())<250) state=0;
        }
            break;
        }
    }




    //    if(DontEnterCircle && circularBorderDANGER.contains(wm->ourRobot[id].pos.loc) && circularBorder2.contains(point2))//circularBorderDANGER.contains(rc.fin_pos.loc))
    //    {
    //        rc.fin_pos.loc=circularBorderDANGER.nearestpoint(point2);//wm->ourRobot[id].pos.loc;//AvoidtoEnterCircle(circularBorderDANGER,point2);
    //        rc.maxSpeed=1.1;
    //    }

    //                     //**//**qDebug()<< "BEFOR ANY CHANGE " << "fin_pos.x  " << rc.fin_pos.loc.x << "  Y  "<<rc.fin_pos.loc.y<< " ------------------------------ STATE = " << state << "    STATE 2 =" << state2;
    //    //**//**qDebug()<< "          " << "ROBOT POS.x  " << wm->ourRobot[id].pos.loc.x << "  Y  "<< wm->ourRobot[id].pos.loc.y;
    //    //**//**qDebug() << "Distance To Fin_Pos =  " << (rc.fin_pos.loc-wm->ourRobot[id].pos.loc).length();

    //**rc.fin_pos.loc=KeepInField(rc);

    //            if(/*((wm->ourRobot[id].pos.loc-opp).length() < 700)*/!OppIsKhoraak && oppIsValid )
    //            {
    rc.fin_pos.loc=AvoidtoEnterCircle(circularBorder,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);//rc.fin_pos.loc);
    //            }
    //rc.fin_pos.loc=AvoidtoEnterCircle(hole1_Offset,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);
    //rc.fin_pos.loc=AvoidtoEnterCircle(hole2_Offset,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);
    //**  if(oppIsValid && OppIsKhoraak) rc.fin_pos.loc=opp;

    //**//**qDebug() << "State Margin = " << statemargin;

    //    if(!OppIsKhoraak)
    //    {
    //        Circle2D c(opp,2*ROBOT_RADIUS+350);
    //        rc.fin_pos.loc=AvoidtoEnterCircle(c,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);
    //    }


    //    bool rich = wm->kn->ReachedToPos(wm->ourRobot[id].pos,rc.fin_pos,10,5);
    //    if(rich) rc.fin_pos.loc=wm->ourRobot[id].pos.loc;

    //     rc.fin_pos.loc=Vector2D(60000,60000);
    //    rc.fin_pos.loc=KeepInField(rc);

    //    rc.fin_pos.loc=Vector2D(MAX_X,MAX_Y);
    // //**//**qDebug() << " DIRECTION ERROR =     " << (rc.fin_pos.dir-wm->ourRobot[id].pos.dir)*(180/3.14);
    //rc.useNav = true;
    //    rc.maxSpeed=1.8*rc.maxSpeed;
    //    rc.isBallObs = false;
    //    rc.isKickObs = false;

    //**                //**//**qDebug()<< "INDEX = " << index << "fin_pos.x  " << rc.fin_pos.loc.x << "  Y  "<<rc.fin_pos.loc.y<< " ------------------------------ STATE = " << state << "    STATE 2 =" << state2;
    ////**//**qDebug()<< "          " << "ROBOT POS.x  " << wm->ourRobot[id].pos.loc.x << "  Y  "<< wm->ourRobot[id].pos.loc.y;
    ////**//**qDebug() << "Distance To Fin_Pos =  " << (rc.fin_pos.loc-wm->ourRobot[id].pos.loc).length();

    //    //**//**qDebug() << " IS INSIDE = " << IsInside << "    UN ACCESSIBLE :" << unAccessible;

    //    //**//**qDebug() << " BALL SIZE : " << wm->balls.size();
    //    //**//**qDebug() << " ROBOT IS IN CORNER " << IsInCorner(wm->ourRobot[id].pos.loc,70) << "     Robot Is In Margin  : " << IsInmargins(wm->ourRobot[id].pos.loc,70) ;
    rc.maxSpeed=1.21*rc.maxSpeed;
    if(AvoidCommand) rc.maxSpeed *= 3.5 ;

    // //**//**qDebug() << " BALL SIZE : " << wm->balls.size();()
    //rc.fin_pos.loc=Vector2D(0,0);

    //**qDebug()<<"temp="<<temp<<","<<"temp2="<<temp2;
    //**qDebug()<<"state"<<state;
    //**qDebug()<<"finalpos("<<rc.fin_pos.loc.x<<","<<rc.fin_pos.loc.y<<")";

    //rc.fin_pos.loc= Vector2D (0,0);

    //qDebug()<<"state2="<<state2;
    //  qDebug() << " State : " << state << " , State 2 : " << state2 << " , State Marg : " << statemargin <<" , MAX SPEED :" << rc.maxSpeed << "  ,  Dist : " << (rc.fin_pos.loc-wm->ourRobot[id].pos.loc).length();
    //    rc.fin_pos.loc = KeepInField(rc);
    //rc.fin_pos.dir=(point2-wm->ourRobot[id].pos.loc).dir().radian();
    return rc;


}



// ==================================================================================
void TacticPush2Goal::addData()
{

    balls.clear();
    balls=wm->balls;//wm->ourRobot[0].pos.loc);

    Vector2D center;//(1667,110);
    center=wm->circularBorder.center();
    circularBorder      .assign(center,900);//+ROBOT_RADIUS);
    circularBorderOut   .assign(center,1500);
    //  circularBorderDANGER.assign(center,520);//+200);
    //   circularBorder2     .assign(center,520);

    hole1.assign(wm->goal1.center(),140);//250
    hole2.assign(wm->goal2.center(),140);
    hole1_Offset.assign(wm->goal1.center(),200);//300
    hole2_Offset.assign(wm->goal2.center(),200);

    //balls.insert(1,wm->ourRobot[1].pos.loc);
    //balls.insert(2,wm->ourRobot[2].pos.loc);

    //    agentsPositive.clear();
    //    agentsPositive.insert(0,wm->ourRobot[4].pos.loc);//wm->ourRobot[0].pos.loc);
    //    agentsPositive.insert(1,wm->ourRobot[5].pos.loc);

    //    agentsPositive.insert(2,wm->oppRobot[0].pos.loc);
}

// ==================================================================================
//Vector2D TacticPush2Goal::findnearest(Vector2D input)
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
void TacticPush2Goal::sortData()
{
    for(int i=0;i<balls.size();i++)
    {
        //for(int j=0;j<segList.size();j++)
        //{
        for(int k=i+1;k<balls.size();k++)
        {
            if( (0.8*((balls.at(i)->pos.loc-circularBorder.center()).length2())+0.2*((balls.at(i)->pos.loc-wm->ourRobot[id].pos.loc).length2()))
                    > (0.8*((balls.at(k)->pos.loc-circularBorder.center()).length2())+0.2*((balls.at(k)->pos.loc-wm->ourRobot[id].pos.loc).length2())) ) balls.swap(i,k);
        }
        //}

    }

}

// =================================================================================
Vector2D TacticPush2Goal::GoOncircle(Vector2D center, double radius)//, Vector2D Object)
{
    //for (int k=0;k<1000;k++)

    Vector2D diff = wm->ourRobot[id].pos.loc - center;
    diff.setDir(diff.dir()+1.1);//50);//.radian() + AngleDeg::deg2rad(5));
    diff.setLength(radius);
    Vector2D point = center + diff;
    return point;

}
// =================================================================================
Vector2D TacticPush2Goal::AvoidtoEnterCircle(Circle2D Ci,Vector2D pnt , Vector2D finPOS)
{

    Avoided=false;
    Segment2D S(pnt,finPOS);
    if(Ci.HasIntersection(S) || Ci.contains(pnt) || Ci.contains(finPOS))
    {
        //        //**//**qDebug() << " HAS INTERSECTION WITH CIRCLE !! DO NOT ENTER THIS CIRCLE PLEASE !!!";
        //                        //**//**qDebug() << " HAS INTERSECTION WITH CIRCLE ( " << Ci.center().x << "," << Ci.center().y << " ) , Radius : " << Ci.radius() ;
        Vector2D c2r = pnt - Ci.center() ;
        Vector2D c2f = finPOS - Ci.center() ;
        c2r.setLength(Ci.radius()+50);
        AngleDeg angle=c2r.dir() - c2f.dir();
        //double ang=c2r.dir().radian() - c2f.dir().radian();
        if (angle.radian() > M_PI) angle -=  AngleDeg(M_PI) *= 2;
        if (angle.radian() < -M_PI) angle += AngleDeg(M_PI) *= 2;

        //if (ang > M_PI) ang -= 2 * M_PI;
        //if (ang < -M_PI) ang += 2 * M_PI;

        ////**//**qDebug() << "c2r DIRECTION = " << c2r.dir().degree() << "c2f DIRECTION = " << c2f.dir().degree() << "   ANGLE  = " << angle.degree() << "    ANG =" << ang*57.32;
        int div=int(Ci.radius()/120);
        //        //**//**qDebug() << "div = " << div ;
        angle=angle/=div;

        angle=c2r.dir() - angle;
        c2r.setDir(angle);// c2f.dir()));
        Vector2D p = Ci.center() + c2r ;
        Avoided=true;
        if((p-pnt).length()<150) AvoidCommand = true ;
        //**               //**//**qDebug() << " Avoided point is : (" << finPOS.x << "," <<finPOS.y << ")  & Corrected is : ("<<
        //**                           p.x << "," << p.y << ")" ;
        return p;
    }
    else
    {
        return finPOS;
    }
}
// ==================================================================================
//        void TacticPush2Goal::FindBall()
//        {
//            for(int i=0;i<balls.size();i++)
//            {

//                c2o = new Segment2D(circularBorderOut.center() , balls.at(i)->pos.loc );
//                temp=0;
//                IsInside     = circularBorder2.contains(balls.at(i)->pos.loc);
//                //        unAccessible = circularBorder .contains(balls.at(i)->pos.loc);

//                //        if(circularBorder2.contains(balls.at(i)->pos.loc))// toop revaal nist
//                //        {
//                //            if( (balls.at(i)->pos.loc-opp).length() <500)            IsInside= true;
//                //            else IsInside= false;
//                //        }
//                if(circularBorder   .contains(balls.at(i)->pos.loc))// toop daakhele mohavvateye emtiaaz manfi ast
//                {
//                    if( ((balls.at(i)->pos.loc-opp).length() <500) && IsInCorner(balls.at(i)->pos.loc,70))            unAccessible=true;
//                    else unAccessible=false;
//                }
//                if(!unAccessible) AllUnAccessible=false;

//                if(!IsInside && !IsInCorner(balls.at(i)->pos.loc,70)) AllIn = false;// All Balls Are in " NO REVAAL " circle
//                if(!IsInCorner(balls.at(i)->pos.loc,70)) AllInCorner=false;
//                //        else if(IsInside) AnyIn = true;

//                if(!IsInside)
//                {
//                    if(!IsInCorner(balls.at(i)->pos.loc,70))
//                    {
//                        index=i;
//                        break;
//                    }

//                }
//                //        if(IsInside) //**//**qDebug()<< " INSIDE " ;
//                //        if(!IsInside) //**//**qDebug()<< "  OUT SIDE " ;
//                //        if(unAccessible) //**//**qDebug() << "UN - ACCESSIBLE" ;
//            }
//            if(AllUnAccessible && AllInCorner)
//            {
//                if( balls.size() > 0 )    index=0;
//            }
//            if(AllIn && balls.size()>0 )
//            {
//                if(oppIsValid)
//                {
//                    furtherByOpp();
//                    if( (balls.at(index)->pos.loc-opp).length() > 500 )            unAccessible=false;
//                }
//                else
//                {
//                    index=balls.size()-1;
//                    if( (balls.at(index)->pos.loc-opp).length() > 500 )            unAccessible=false;
//                }
//            }
//        }
// ==================================================================================


//shahin
void TacticPush2Goal::FindBall()
{
    for(int i=0;i<balls.size();i++)
    {
        temp=0;
        temp2=0;

        if(!(circularBorder.contains(balls.at(i)->pos.loc)))
        {

            index=i;

            temp=1;



            if(!(circularBorderOut.contains(balls.at(i)->pos.loc)))
            {
                temp2=1;
            }

            break;
        }


    }






}

//shahout



//=======================================================================================

void TacticPush2Goal::FindHole()
{
    Vector2D dist1 = balls.at(index)->pos.loc- hole1.center() ;
    Vector2D dist2 = balls.at(index)->pos.loc- hole2.center() ;
    if(dist2.length2()>dist1.length2())
    {
        vec2goal = (hole1.center() - point2);
    }
    else
    {
        vec2goal = (hole2.center() - point2);
    }
}
// ==================================================================================

void TacticPush2Goal::furtherByOpp()
{
    double dist1 = 0;
    for(int j=0;j<balls.size();j++)
    {
        if(((balls.at(j)->pos.loc-opp).length2()) > dist1 )
        {
            dist1=(balls.at(j)->pos.loc-opp).length2();
            index=j;
        }
        //        //**//**qDebug() << " DIST  = " << dist1 ;
    }
}

// ==================================================================================

Vector2D TacticPush2Goal::KeepInField(RobotCommand rc)
{
    if(rc.fin_pos.loc.x > MAX_X-ROBOT_RADIUS)
    {
        rc.fin_pos.loc = Vector2D(MAX_X-1.5*ROBOT_RADIUS,rc.fin_pos.loc.y);
        //**              //**//**qDebug() << " X > MAX_X !! ";
    }
    else if(rc.fin_pos.loc.x < MIN_X+ROBOT_RADIUS+100)
    {
        rc.fin_pos.loc = Vector2D(MIN_X+1.5*ROBOT_RADIUS,rc.fin_pos.loc.y);
        //**               //**//**qDebug() << " X < MIN_X !! ";
    }

    if(rc.fin_pos.loc.y > MAX_Y+ROBOT_RADIUS)
    {
        rc.fin_pos.loc = Vector2D(rc.fin_pos.loc.x,MAX_Y-1.5*ROBOT_RADIUS);
        //**              //**//**qDebug() << " Y > MAX_Y !! ";
    }
    else if(rc.fin_pos.loc.y < MIN_Y-ROBOT_RADIUS)
    {
        rc.fin_pos.loc = Vector2D(rc.fin_pos.loc.x,MIN_Y+1.5*ROBOT_RADIUS);
        //**               //**//**qDebug() << " Y < MIN_Y !! ";
    }

    return rc.fin_pos.loc;

}

// ==================================================================================

bool TacticPush2Goal::IsInCorner(Vector2D pnt,double margin)
{
    //
    //    double margin=100;
    if(((pnt.x > MAX_X-margin) || (pnt.x < MIN_X+margin)) && ((pnt.y > MAX_Y-margin) || (pnt.y < MIN_Y+margin)))
    { return true;
        //**               //**//**qDebug() << " IS IN CORNER ";
    }

    else return false;
}

// ==================================================================================

bool TacticPush2Goal::IsInmargins(Vector2D pnt, double margin)
{
    //
    if((pnt.x > MAX_X-margin) || (pnt.x < MIN_X+margin) || (pnt.y > MAX_Y-margin) || (pnt.y < MIN_Y+margin))
    {
        //**               //**//**qDebug() << " IS IN MARGIN ";
        return true;
    }
    else return false;
}

// ==================================================================================
// ==================================================================================
