#include "tacticconfront.h"

TacticConfront::TacticConfront(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticConfront", worldmodel, parent)
{
    count=0;

    obs=0;
    rcpast=Vector2D(0,0);
    DoNotEnterOpposedField=false;//true;//
    CanKickOpp = true;

    OppIsValid = true;

    MAX_X = 2570; MIN_X=0;
    MAX_Y = 1950; MIN_Y=-1520;//750;


    mean_x=(MAX_X+MIN_X) / 2;
    mean_y=(MAX_Y+MIN_Y) / 2;

    statemargin=0;

    OffsetforSeg=Vector2D(150,0);
}

RobotCommand TacticConfront::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    OppIsValid=false;//wm->theirRobot.IsValid;//wm->ourRobot[8].isValid;
    if(OppIsValid) Opp=wm->ourRobot[6].pos.loc;//wm->ourRobot[8].pos.loc;
    else Opp=Vector2D(100000,100000);
    OppIsInhisField = true;
    rc.fin_pos.loc=origin;
    rc.fin_pos.dir=(origin2-origin).dir().radian();
    rc.maxSpeed = 1.4;
    nindex=-1;
    pindex=-1;
    int object;
    reach=false;




 //   qDebug()<<"before add data ...";
    addData();
  //  qDebug()<<"after add data ...";
    addseg();
//    mergeData();
  //  qDebug()<<"before sort data ...";
    sortData();
 //   qDebug()<<"after sort data ...";

//    object=findnearestObject(wm->positiveShapes,wm->ourRobot[id].pos.loc);
//    if(object!=-1) ObsC=Circle2D(wm->positiveShapes.at(object).position,(wm->positiveShapes.at(object).roundedRadios+ROBOT_RADIUS+50));
//    rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

//    object=findnearestObject(wm->negativeShapes,wm->ourRobot[id].pos.loc);
//    if(object!=-1) ObsC=Circle2D(wm->negativeShapes.at(object).position,(wm->negativeShapes.at(object).roundedRadios+ROBOT_RADIUS+50));
//    rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

    //Line2D segline = testseg->line();
    //double y=segline.getX(500);
    //qDebug() << y;

    //    Vector2D proj=testseg->nearestPoint(wm->ball.pos.loc);
    //    qDebug() << proj.x<< "------ Y = " << proj.y ; // length();//atan(perp.a()/perp.b())*180/M_PI;
    //   qDebug() << wm->ball.pos.loc.x<< "------ Y = " << wm->ball.pos.loc.y ;
    //    testseg->existIntersection();





    //---------------------------TRANSFER NEGATIVE OBJECTS OVER THE LINE ---------------

    // vvvvvv Choose The Negative Object To Transfer ------------------------------


    for(int i=0;i<agentsNegative.size();i++)
    {


        //if(obstacle)wm->kn->)
        //{continue; }
        //origin.x=origin.x*(2*agentsNegative.at(i).goalRegion - 1 );
        o2o = new Segment2D(origin , agentsNegative.at(i).position );//=Segment2D o2o;
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

        if(!IsOverTheLine)//in our field
        {
            nindex=i;
            //qDebug() << 2*agentsNegative.at(i).goalRegion - 1 ;
            //qDebug() << agentsNegative.at(i).x << " Y ---- " << agentsNegative.at(i).y;// TOOOOOOOOOOOOOOOOOOOSHE !!!!!!!" << index ;
            //goalRegion=agentsNegative.at(i).goalRegion;
            temp=1;
            break;
        }

    }
    //^^^^^^^^^^^ choose The Object To Transfer --------------------


    //**if(index !=-1)
    //**{
     //**   point2 = agentsNegative.at(index).position;
      //**  rad=agentsNegative.at(index).roundedRadios; // ????? Int Rad ??? Or Double ?!?!?!?!?
       //** Vector2D nrstpnt = findnearest(point2);
       //** Vector2D diff2 = nrstpnt - point2;// wm->ourRobot[id].pos.loc ;
//        bool reach=false;
        // agentsNegative.

        if(temp!=0 && nindex!=-1)
        {

            qDebug() <<"nindex : "<<nindex;
            point2 = agentsNegative.at(nindex).position;
            rad=agentsNegative.at(nindex).roundedRadios; // ????? Int Rad ??? Or Double ?!?!?!?!?
            Vector2D nrstpnt = findnearest(point2);
            Vector2D diff2 = nrstpnt - point2;// wm->ourRobot[id].pos.loc ;
    //        bool reach=false;
            // agentsNegative.



            switch(state)
            {

            case 0:
                    {//Go Behind the Object
                    Vector2D space2=diff2;
                    //int side = agentsNegative.at(index).goalRegion;
                    space2.setLength(300);
                    //space2.setDir(space2.dir() + M_PI*side);
                    rc.maxSpeed=1.2;
                    rc.useNav = true;

                    //shahin
                    rc.fin_pos.loc=point2  - space2;

                    ObsC=Circle2D(wm->negativeShapes.at(nindex).position,(wm->negativeShapes.at(nindex).roundedRadios+ROBOT_RADIUS+10));
                    rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

                    rc.fin_pos.dir=diff2.dir().radian();

                    //shahout

//                    object=findnearestObject(wm->negativeShapes,wm->ourRobot[id].pos.loc);
//                    if(object!=-1) ObsC=Circle2D(wm->negativeShapes.at(object).position,(wm->negativeShapes.at(object).roundedRadios+ROBOT_RADIUS+200));
//                    rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

                    reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,100);
                    if(reach) state = 1;

            }
                break;
            case 1:{//Ready to Push

                rc.useNav = false;
                rc.maxSpeed=1.2;
                int side = -1;//agentsNegative.at(index).goalRegion;
                //space3=diff2;
                //space3.setlength(100+robotradius);
                //space3.x=(side)*(100+ROBOT_RADIUS)*(diff2.x)/(diff2.length());

                //shahin

                Vector2D space=diff2;
                space.setLength(100+ROBOT_RADIUS);
                rc.fin_pos.loc=point2 - space;

                //shahout

                //**rc.fin_pos.loc.x=point2.x +(side)*(100+ROBOT_RADIUS)*(diff2.x)/(diff2.length());
                //**rc.fin_pos.loc.y=point2.y +(side)*(100+ROBOT_RADIUS)*(diff2.y)/(diff2.length());
                rc.fin_pos.dir=diff2.dir().radian();
                if(((wm->ourRobot[id].pos.loc-point2).length())>600)
                {

                    state=0;
                }
                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,70);
                if(reach)
                    state = 2;
            }
                break;
            case 2:{//Push
                //Vector2D diff2 = region2.center() - wm->ourRobot[id].pos.loc ;
                rc.useNav = false;
                rc.maxSpeed=1;
                //if(diff2.length() > 1500) diff2.setLength(1500);
                diff2.setLength(300);

                rc.fin_pos.loc=point2 + diff2;//5;




               object=findnearestObject(wm->positiveShapes,agentsNegative.at(nindex).position);
               if(object!=-1) ObsC=Circle2D(wm->positiveShapes.at(object).position,(wm->positiveShapes.at(object).roundedRadios+ROBOT_RADIUS+10));
               rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);








                rc.fin_pos.dir=diff2.dir().radian();

                if(((wm->ourRobot[id].pos.loc-point2).length())>400)
                {
                    state = 0 ;
                }
                if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length())<100)
                {
                    state = 0;
                }

                //                Vector2D o2r = ( point2 - wm->ourRobot[id].pos.loc );
                //                if(fabs(wm->ourRobot[id].pos.dir - o2r.dir().radian()) > AngleDeg::deg2rad(30))
                //                {
                //                    qDebug() << " !!!! Out OF Direction !!!! " ;
                //                    state=1;//4;
                //                }


                //shahin

                //detecting blocked negative object////////////////////////////////////////////////////////////////
                if(counter%100==0)
                {

                    unegative = agentsNegative.at(nindex).position;


                    if((lastunegative-unegative).length()<200)
                    {
                        state=4;
                    }



                    lastunegative=unegative;

                    counter=0;



               }


                counter++;


                //shahout

                //..///////////////////////////////////////////////////////////////////////////////////////////////

               }


                break;



            case 3:{//Release

                // if(testseg->existIntersection(*o2o))
                //{
            //    qDebug() << " INNNNNNNNNNNN SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIDE !!!";
                count++;
                //if(index==agentsR1.size()) rc.fin_pos.loc=Vector2D(0,0);
                if(temp==0)
                {
                    rc.fin_pos.loc=origin;//Vector2D(1500,1500);// ?????????????????????
                    break;
                }
                //agentsR1.takeFirst();
                //index++;
                // }
                //if(reach)
                state = 0;
            }

                break;


            case 4://return to your origine because there is a negative object which is blocked...
            {

                rc.fin_pos.loc=origin;

                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,100);

                if(reach) state = 0;

                  }

                break;




            }
        }
    // ---------------------------------------------------------------------------------

    //-----------------------   TRANSFER POSITIVE OBJECTS OVER THE LINE    -------------
    if(temp==0)
    {
       //         qDebug() << "---------TRANSFER POSITIVE  ---------";
        for(int i=0;i<agentsPositive.size();i++)
        {
            //if(obstacle)wm->kn->)
            //{continue; }
            //r2o = new Segment2D(Vector2D(wm->ourRobot[id].pos.loc) , Vector2D(agentsPositive.at(i).pos));
            //qDebug() << 2*agentsPositive.at(i).goalRegion - 1 ;


            o2o = new Segment2D(origin2 , Vector2D(agentsPositive.at(i).position));
            o3o = new Segment2D(origin , Vector2D(agentsPositive.at(i).position));


            IsOverTheLine=false;
            IsOverThesafeline=false;
            temp2=0;

            for(int j=0;j<segList.size(); j++)
            {
                if(segList.at(j).existIntersection(*o2o))
                {
                    IsOverTheLine=true;
                    break;
                }

            }

            if(!(safeline->existIntersection(*o3o)))
            {
                    IsOverThesafeline=true;//the positive object is in its final position...
            }


            if(!(IsOverThesafeline))//between the frontier and our safeline
            {
                 pindex=i;

                if(IsOverTheLine)
                {

                    temp2=1;
                    break;

                }





            }
//                qDebug() << agentsPositive.at(i).x << " Y ---- " << agentsPositive.at(i).y;// TOOOOOOOOOOOOOOOOOOOSHE !!!!!!!" << index ;
                //goalRegion=agentsPositive.at(i).goalRegion;
            }



        if(pindex!=-1) // Positive Objects are in field
        {
            Vector2D point2 = agentsPositive.at(pindex).position;

            rad=agentsPositive.at(pindex).roundedRadios;

            Vector2D nrstpnt = findnearest(point2);

            Vector2D diff2 = nrstpnt - point2;

            Vector2D safelinenrstpnt = safeline->nearestPoint(agentsPositive.at(pindex).position);

            Vector2D diff3 = safelinenrstpnt - point2 ;

            bool reach=false;

            if(temp2!=0)
            {
                    switch(state)
                    {
                    case 0:{ //Go Behind the Object

                        Vector2D space2=diff3;
                        space2.setLength(300);
                        rc.maxSpeed=1.5;
                        rc.useNav = true;


                        //shahin

                        rc.fin_pos.loc = point2 - space2;


                        ObsC=Circle2D(wm->positiveShapes.at(pindex).position,(wm->positiveShapes.at(pindex).roundedRadios+ROBOT_RADIUS+10));
                        rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

                        rc.fin_pos.dir=diff3.dir().radian();

                        //shahout

                        //**object=findnearestObject(wm->positiveShapes,wm->ourRobot[id].pos.loc);
                        //**if(object!=-1) ObsC=Circle2D(wm->positiveShapes.at(object).position,(wm->positiveShapes.at(object).roundedRadios+ROBOT_RADIUS+150));
                        //**rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

                        reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,50);
                        if(reach) state = 1;

                    }
                        break;
                    case 1:{//Ready to Push

                        rc.useNav = false;
                        rc.maxSpeed=1.2;
                        int side = -1;//agentsPositive.at(index).goalRegion;
                        //**rc.fin_pos.loc.x=point2.x +(side)*(100 + ROBOT_RADIUS)*(diff2.x)/(diff2.length());
                       //** rc.fin_pos.loc.y=point2.y +(side)*(100 + ROBOT_RADIUS)*(diff2.y)/(diff2.length());



                        Vector2D space=diff3;

                        space.setLength(100+ROBOT_RADIUS);

                        rc.fin_pos.loc = point2 - space;

                        rc.fin_pos.dir=diff3.dir().radian();


                        if(((wm->ourRobot[id].pos.loc-point2).length())>600) state=0;

                        reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,70);
                        if(reach)
                            state = 2;
                    }
                        break;
                    case 2:{//Push

                        diff3.setLength(300);

                        rc.fin_pos.loc = point2 + diff3;

                        rc.fin_pos.dir=diff3.dir().radian();
                        //Vector2D diff2 = region2.center() - wm->ourRobot[id].pos.loc ;
                        rc.useNav = false;
                        rc.maxSpeed=1;
                        //if(diff2.length() > 1500) diff2.setLength(1500);
                        if(((wm->ourRobot[id].pos.loc-point2).length())>400) state=0;
                        if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc/*point2*/).length())<100) state=0;

                        //                        Vector2D o2r = ( point2 - wm->ourRobot[id].pos.loc );
                        //                        if(fabs(wm->ourRobot[id].pos.dir - o2r.dir().radian()) > AngleDeg::deg2rad(20))
                        //                        {
                        //                            qDebug() << " !!!! Out OF Direction !!!! " ;
                        //                            state=1;//4;
                        //                        }


                    }
                        break;
                    case 3:{//Release

                        // if(testseg->existIntersection(*o2o))
                        //{
         //               qDebug() << " INNNNNNNNNNNN SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIDE !!!";
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




                switch(state)
                {
                case 0:{ //Go Behind the Object

                    Vector2D space2=diff3;
                    space2.setLength(300);
                    rc.maxSpeed=1.2;
                    rc.useNav = true;


                    //shahin

                    rc.fin_pos.loc = point2 - space2;


                    ObsC=Circle2D(wm->positiveShapes.at(pindex).position,(wm->positiveShapes.at(pindex).roundedRadios+ROBOT_RADIUS+10));
                    rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

                    rc.fin_pos.dir=diff3.dir().radian();

                    //shahout

                    //**object=findnearestObject(wm->positiveShapes,wm->ourRobot[id].pos.loc);
                    //**if(object!=-1) ObsC=Circle2D(wm->positiveShapes.at(object).position,(wm->positiveShapes.at(object).roundedRadios+ROBOT_RADIUS+150));
                    //**rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

                    reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,50);
                    if(reach) state = 1;

                }
                    break;
                case 1:{//Ready to Push

                    rc.useNav = false;
                    rc.maxSpeed=1;
                    int side = -1;//agentsPositive.at(index).goalRegion;
                    //**rc.fin_pos.loc.x=point2.x +(side)*(100 + ROBOT_RADIUS)*(diff2.x)/(diff2.length());
                   //** rc.fin_pos.loc.y=point2.y +(side)*(100 + ROBOT_RADIUS)*(diff2.y)/(diff2.length());



                    Vector2D space=diff3;

                    space.setLength(100+ROBOT_RADIUS);

                    rc.fin_pos.loc = point2 - space;

                    rc.fin_pos.dir=diff3.dir().radian();


                    if(((wm->ourRobot[id].pos.loc-point2).length())>600) state=0;

                    reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,70);
                    if(reach)
                        state = 2;
                }
                    break;
                case 2:{//Push

                    diff3.setLength(300);

                    rc.fin_pos.loc = point2 + diff3;

                    rc.fin_pos.dir=diff3.dir().radian();
                    //Vector2D diff2 = region2.center() - wm->ourRobot[id].pos.loc ;
                    rc.useNav = false;
                    rc.maxSpeed=1;
                    //if(diff2.length() > 1500) diff2.setLength(1500);
                    if(((wm->ourRobot[id].pos.loc-point2).length())>400) state=0;
                    if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc/*point2*/).length())<100) state=0;

                    //                        Vector2D o2r = ( point2 - wm->ourRobot[id].pos.loc );
                    //                        if(fabs(wm->ourRobot[id].pos.dir - o2r.dir().radian()) > AngleDeg::deg2rad(20))
                    //                        {
                    //                            qDebug() << " !!!! Out OF Direction !!!! " ;
                    //                            state=1;//4;
                    //                        }


                }
                    break;
                case 3:{//Release

                    // if(testseg->existIntersection(*o2o))
                    //{
   //                 qDebug() << " INNNNNNNNNNNN SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIDE !!!";
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

    }
    }



        //----------------------------------------------------------------------------

    //    qDebug()<< /*"fin_pos.x  " << rc.fin_pos.loc.x << "  Y  "<<rc.fin_pos.loc.y<< " ------------------------------ */"STATE = " << state ;
    //    qDebug() << "USE NAVIGATION IS : " << rc.useNav;


//    // =================== IS IN MARGINS ==================================??????????????????????
//    if(index != -1)
//    {
////        Vector2D nrstpnt = findnearest(point2);
////        Vector2D diff2 = nrstpnt - point2;
//        int mrgn=200;
//        Vector2D dlta;
//        if(false)//IsInmargins(point2,mrgn))
//        {
//            int side = ((point2.x-mean_x)/abs(point2.x-mean_x))*((point2.y-mean_y)/abs(point2.y-mean_y));
//            if(point2.x > MAX_X-mrgn || point2.x < MIN_X+mrgn) {
//                side *= ((point2.y-mean_y)/abs(point2.y-mean_y));
//                dlta=Vector2D(0,side*(ROBOT_RADIUS+20));}
//            else if(point2.y > MAX_Y-mrgn || point2.y < MIN_Y+mrgn) {
//                side *=((point2.x-mean_x)/abs(point2.x-mean_x));
//                dlta=Vector2D(side*(ROBOT_RADIUS+20),0);}
//            switch(statemargin)
//            {
//            case 0:{

//                rc.fin_pos.loc=point2+dlta;
//                rad += ROBOT_RADIUS + 50;
////                    int rad = 70+ROBOT_RADIUS;
//                Circle2D c(point2,rad);
//                rc.fin_pos.loc=AvoidtoEnterCircle(c,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

////                qDebug()<< "In Margins Pos  : ball = ( " << point2.x << ","<< point2.y << ")";
////                qDebug()<< "In Margins Pos  : delta = ( " << dlta.x << ","<< dlta.y << ")";
////                qDebug()<< "In Margins Pos  : fin_pos = ( " << rc.fin_pos.loc.x << ","<<rc.fin_pos.loc.y << ")";
////                qDebug()<< "In Margins Pos  : Robot = ( " << wm->ourRobot[id].pos.loc.x << ","<<wm->ourRobot[id].pos.loc.y << ")";
//                rc.fin_pos.dir=dlta.dir().radian()-side*M_PI/2;
//                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos,rc.fin_pos,20,7);
//    //                            wm->ourRobot[id].pos.loc,rc.fin_pos.loc,200);
////                qDebug() << "dist To final Pos : " << (wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length();
////                qDebug() << " Avoided : " << Avoided << "     reach" << reach;
//                if(reach) statemargin = 1;
//            }
//                break;

//            case 1:{
//                rc.fin_pos.dir = dlta.dir().radian() - side*0.9*M_PI ;
//                rc.fin_pos.loc=point2-dlta;
////                qDebug() << "Fin_POS . dir = " << AngleDeg::rad2deg(rc.fin_pos.dir) << " ROBOT . dir = " <<  AngleDeg::rad2deg(wm->ourRobot[id].pos.dir);
//                if(((wm->ourRobot[id].pos.loc-point2).length())>300) statemargin=0;
//                double delta_ang=wm->ourRobot[id].pos.dir-rc.fin_pos.dir;
//                if (delta_ang >  M_PI) delta_ang -= (M_PI * 2);
//                if (delta_ang < -M_PI) delta_ang += (M_PI * 2);
//                if(fabs(delta_ang) < AngleDeg::deg2rad(10)) statemargin=0;
//                rc.maxSpeed=1.7;
//    //                bool chighz=wm->kn->ReachedToPos(wm->ourRobot[id].pos,rc.fin_pos,20,10);
//    //                if(chighz) statemargin=0;
//    //                if((wm->ourRobot[id].pos.loc.dir()-rc.fin_pos.dir)<AngleDeg::deg2rad(10)) statemargin=0;
//    //                if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length())<250) state=0;
//            }
//                break;
//            }
//        }
//    }

    //======================================================================

    if(OppIsValid)
    {
        Segment2D Opp2O(Opp , origin2);

        for(int j=0;j<seglistOffset.size(); j++)
        {
            if(seglistOffset.at(j).existIntersection(Opp2O))
            {
                OppIsInhisField = false;
                break;
            }
        }
    }

    if(OppIsValid && OppIsInhisField && (wm->ourRobot[id].pos.loc-Opp).length() < 1000 ) DoNotEnterOpposedField=true;
    if(!OppIsValid || (wm->ourRobot[id].pos.loc-Opp).length() > 1000 ) DoNotEnterOpposedField=false;
    if(DoNotEnterOpposedField)
    {
        // DO NOT ENTER OPPOSED Field
        Segment2D fin2o(origin , rc.fin_pos.loc );

        for(int j=0;j<segList.size(); j++)
        {
            if(segList.at(j).existIntersection(fin2o))
            {
                rc.fin_pos.loc=segList.at(j).nearestPoint(rc.fin_pos.loc);
  //              qDebug() << "DO NOT ENTER OPPOSED REGION";
                break;
            }
        }
    }

   // Circle2D c(Opp,ROBOT_RADIUS*3+100);
   // rc.fin_pos.loc=AvoidtoEnterCircle(c,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

    //rc.useNav=true;

    if(!OppIsInhisField )
    {
        rc.fin_pos.loc = Opp;//wm->ourRobot[8].pos.loc;
    }
  //  qDebug() << "  S T A T E  :  " << state;

//    qDebug() << " OPP Is In : (" << Opp.x << "," << Opp.y << " ) , Opp Is Valid Is : " << OppIsValid << " Opp Is In His Field Is : " << OppIsInhisField ;
//    rc.fin_pos.loc.x=rcpast.x + 0.6*(rc.fin_pos.loc.x-rcpast.x);
//    rc.fin_pos.loc.y=rcpast.y + 0.6*(rc.fin_pos.loc.y-rcpast.y);
//    rcpast=rc.fin_pos.loc;
//    qDebug() << "RC FIN POS : (" << rc.fin_pos.loc.x << "," << rc.fin_pos.loc.y << ")";
    //    qDebug() << " DIST To Final Pos = " << (wm->ourRobot[id].pos.loc - rc.fin_pos.loc).length();
    rc.maxSpeed /= 1.5;
    rc.useNav=false;
    rc.isBallObs = false;
    rc.isKickObs = true;


    //shahin


    //shahout
//    rc.fin_pos.loc=origin;

    qDebug()<<"temp="<<temp<<"......temp2="<<temp2;
    return rc;


}
// ==================================================================================
void TacticConfront::addData()
{


    //qDebug()<<"add...";

    if(wm->negativeShapes.size()>0)
    {
        agentsNegative.clear();
        for(int i=0;i<wm->negativeShapes.size();i++)
        {
            IsInmargin=IsInmargins(wm->negativeShapes.at(i).position,300);

            if(!IsInmargin)
                agentsNegative.push_back(wm->negativeShapes.at(i));
        }

    }

    else
    {
        agentsNegative.clear();
        agentsNegative=wm->negativeShapes;

    }




    if(wm->positiveShapes.size()>0)
    {
        agentsPositive.clear();
        for(int i=0;i<wm->positiveShapes.size();i++)
        {
            IsInmargin=IsInmargins(wm->positiveShapes.at(i).position,300);

            if(!IsInmargin)
                agentsPositive.push_back(wm->positiveShapes.at(i));
        }
    }

   else
    {
        agentsPositive.clear();
        agentsPositive=wm->positiveShapes;
    }




    // agentsPositive=wm->positiveShapes;



 //   origin=wm->endPoint;//Vector2D(2200,0);

    origin=wm->endPoint;
    origin2=Vector2D(0,0);//A point from the field of enemy...

    //shahin

    safeorigine1=Vector2D(origin.x-200,-1250);
    safeorigine2=Vector2D(origin.x-200,1480);

    safeline = new Segment2D(safeorigine1 , safeorigine2);

     //shahout
}
// ==================================================================================
void TacticConfront::addseg()
{

    segList.clear(); // SO IMPORTANT !!!
    segList=wm->borders;
    for(int i=0;i<wm->borders.size();i++)
    {
        Segment2D seg(wm->borders.at(i).origin()+OffsetforSeg,wm->borders.at(i).terminal()+OffsetforSeg);
        seglistOffset.push_back(seg);
//        qDebug() << " JAFARAM " ;
//        qDebug() << wm->borders.at(i).origin().x << wm->borders.at(i).origin().y << " , Terminal : "  << wm->borders.at(i).terminal().x << wm->borders.at(i).terminal().y ;
    }

}
// ==================================================================================
Vector2D TacticConfront::AvoidtoEnterCircle(Circle2D Ci, Vector2D pnt, Vector2D finPOS)
{
    //    Avoided=false;
    Segment2D S(pnt,finPOS);

    if(Ci.HasIntersection(S) || Ci.contains(pnt) || Ci.contains(finPOS))
    {
        //        qDebug() << " HAS INTERSECTION WITH CIRCLE !! DO NOT ENTER THIS CIRCLE PLEASE !!!";
//                qDebug() << " HAS INTERSECTION WITH CIRCLE ( " << Ci.center().x << "," << Ci.center().y << " ) , Radius : " << Ci.radius() ;
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
// ==================================================================================
bool TacticConfront::IsInmargins(Vector2D pnt, double margin)
{
    {
        //
        if((pnt.x > MAX_X-margin) || (pnt.x < MIN_X+margin) || (pnt.y > MAX_Y-margin) || (pnt.y < MIN_Y+margin))
        {
            qDebug() << " IS IN MARGIN ";
            return true;
        }
        else return false;
    }
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

     //qDebug()<<"sort...";

////////////////////////////////////////sorting positive agents...

    for(int i=0;i<agentsPositive.size();i++)
    {
        //for(int j=0;j<segList.size();j++)
        //{
//        for(int k=i+1;k<agentsPositive.size();k++)
//        {
//            if(( (agentsPositive.at(i).position-wm->ourRobot[id].pos.loc).length2() +(agentsPositive.at(i).position-findnearest(agentsPositive.at(i).position)).length2())
//                    > ((agentsPositive.at(k).position-wm->ourRobot[id].pos.loc).length2() +((agentsPositive.at(i).position- findnearest(agentsPositive.at(k).position)).length2() )) ) agentsPositive.swap(i,k);
//        }
        //}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                for(int k=i+1;k<agentsPositive.size();k++)
//                {
//                    if(( (agentsPositive.at(i).position-wm->ourRobot[id].pos.loc).length2())
//                            > ((agentsPositive.at(k).position-wm->ourRobot[id].pos.loc).length2()) ) agentsPositive.swap(i,k);
//                }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                for(int k=i+1;k<agentsPositive.size();k++)
//                {
//                    if(( (agentsPositive.at(i).position-wm->ourRobot[id].pos.loc).length2())
//                            < ((agentsPositive.at(k).position-wm->ourRobot[id].pos.loc).length2()) ) agentsPositive.swap(i,k);
//                }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// //       for(int j=0;j<segList.size();j++)
// //      {
//        for(int k=i+1;k<agentsPositive.size();k++)
//        {
//            if(  ( (agentsPositive.at(i).position-findnearest(agentsPositive.at(i).position)).length2())
//                    >  (( agentsPositive.at(k).position- findnearest(agentsPositive.at(k).position)).length2())  )   agentsPositive.swap(i,k);


//        }

//    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        for(int j=0;j<segList.size();j++)
//        {
        for(int k=i+1;k<agentsPositive.size();k++)
        {
            if(( -(agentsPositive.at(i).position-wm->ourRobot[id].pos.loc).length2() +(agentsPositive.at(i).position-safeline->nearestPoint(agentsPositive.at(i).position)).length2())
                    < (-(agentsPositive.at(k).position-wm->ourRobot[id].pos.loc).length2() +(agentsPositive.at(i).position-safeline->nearestPoint(agentsPositive.at(k).position)).length2() ) ) agentsPositive.swap(i,k);
        }
        }
////////////////////////////////////////sorting negative agents...


    for(int i=0;i<agentsNegative.size();i++)
    {
        //for(int j=0;j<segList.size();j++)
        //{
        for(int k=i+1;k<agentsNegative.size();k++)
        {
//            if( (agentsNegative.at(i)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(i))*/).length2()
//                    > (agentsNegative.at(k)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(k))*/).length2() ) agentsNegative.swap(i,k);

            if(( -(agentsNegative.at(i).position-wm->ourRobot[id].pos.loc).length2() +(agentsNegative.at(i).position-findnearest(agentsNegative.at(i).position)).length2())
                    < (-(agentsNegative.at(k).position-wm->ourRobot[id].pos.loc).length2() +((agentsNegative.at(k).position- findnearest(agentsNegative.at(k).position)).length2() )) ) agentsNegative.swap(i,k);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                for(int k=i+1;k<agentsNegative.size();k++)
//                {
//        //            if( (agentsNegative.at(i)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(i))*/).length2()
//        //                    > (agentsNegative.at(k)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(k))*/).length2() ) agentsNegative.swap(i,k);

//                    if(( (agentsNegative.at(i).position-wm->ourRobot[id].pos.loc).length2())
//                            > ((agentsNegative.at(k).position-wm->ourRobot[id].pos.loc).length2()) ) agentsNegative.swap(i,k);

//                }

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                for(int k=i+1;k<agentsNegative.size();k++)
//                {
//        //            if( (agentsNegative.at(i)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(i))*/).length2()
//        //                    > (agentsNegative.at(k)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(k))*/).length2() ) agentsNegative.swap(i,k);

//                    if(( (agentsNegative.at(i).position-wm->ourRobot[id].pos.loc).length2())
//                            < ((agentsNegative.at(k).position-wm->ourRobot[id].pos.loc).length2()) ) agentsNegative.swap(i,k);

//                }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        for(int k=i+1;k<agentsNegative.size();k++)
//        {
////            if( (agentsNegative.at(i)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(i))*/).length2()
////                    > (agentsNegative.at(k)-wm->ourRobot[id].pos.loc/*findnearest(agentsNegative.at(k))*/).length2() ) agentsNegative.swap(i,k);

//            if(  ( (agentsNegative.at(i).position-findnearest(agentsNegative.at(i).position)).length2())
//                    <  ( (agentsNegative.at(k).position- findnearest(agentsNegative.at(k).position)).length2() ) ) agentsNegative.swap(i,k);


//        //}
//        }

        }



}

}



// ==================================================================================
// ==================================================================================
