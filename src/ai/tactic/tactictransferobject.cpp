#include "tactictransferobject.h"

TacticTransferObject::TacticTransferObject(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticTransferObject", worldmodel, parent)
{

    state=0;
    rcpast=Vector2D(0,0);
    statemargin=0;
    //index=0;
    firstInit=true;//false;

    MAX_X = 3500; MIN_X=0;
    MAX_Y = 1750; MIN_Y=-1750;//750;


    mean_x=(MAX_X+MIN_X) / 2;
    mean_y=(MAX_Y+MIN_Y) / 2;

}

RobotCommand TacticTransferObject::getCommand()
{
    AllInMargin=true;
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    rc.useNav=true;
    rc.maxSpeed = 1;
    rc.fin_pos.loc=wm->endPoint;//Vector2D(300,0);
    int object;

    addData();
    mergeData();
    sortData();


    //    if(wm->balls.size() > 0)
    //    {
    //            qDebug()<< " BALLL . X = " << wm->balls.at(0)->pos.loc.x << " BALLL . Y = " <<  wm->balls.at(0)->pos.loc.y;
    //                    qDebug() << " MAX x : " << region[1].maxX() << "  MIN X : " << region[1].minX() ;
    //                    qDebug() << " MAX y : " << region[1].maxY() << "  MIN y : " << region[1].minY() ;
    //            if(region[0].IsInside(wm->balls.at(0)->pos.loc)) qDebug() << " THE BALLLLL ISSS INNNNN SIDE !!!!!!!!!!!!!!!!!!!!!!1";
    //    }
    index = -1;
    for(int i=0;i<mergedList.size();i++)
    {
//        qDebug() << i << " AT : (" << mergedList.at(i).pos.x << "," << mergedList.at(i).pos.y << ")";
        temp=0;
        if(!region[mergedList.at(i).goalRegion].IsInside(mergedList.at(i).pos) && !IsInmargins(mergedList.at(i).pos,300))
        {
            //qDebug() <<" OBJECT :    " <<  mergedList.at(i).pos.x << " ------ Y = " << mergedList.at(i).pos.y;// TOOOOOOOOOOOOOOOOOOOSHE !!!!!!!" << index ;
//            AllInMargin=false;
            index=i;
            goalRegion=mergedList.at(i).goalRegion;
            temp=1;
            break;
        }

    }

    for(int i=0; i<mergedList.size(); i++)
    {
        if(!IsInmargins(mergedList.at(i).pos,300))
        {
            AllInMargin=false;
        }
    }


    if(AllInMargin)
    {
        for(int i=0;i<mergedList.size();i++)
        {
            if(!region[mergedList.at(i).goalRegion].IsInside(mergedList.at(i).pos))
            {
                index=i;
                goalRegion=mergedList.at(i).goalRegion;
                break;
            }
        }
    }
//    if(index ==-1)
//    {
//        for(int i=0;i<wm->Chasbideh.size(); i++)
//        {
//            if(!region[0].IsInside(wm->Chasbideh.at(i).position) && !region[1].IsInside(wm->Chasbideh.at(i).position))
//            {
//                //qDebug() <<" OBJECT :    " <<  mergedList.at(i).pos.x << " ------ Y = " << mergedList.at(i).pos.y;// TOOOOOOOOOOOOOOOOOOOSHE !!!!!!!" << index ;
//                index=i;
//                goalRegion=0;//mergedList.at(i).goalRegion;
//                temp=1;
//                break;
//            }
//        }
//    }




//     qDebug() << mergedList.size() << " MERGED SIZE " ;
    if(index != -1)
    {

        Vector2D point2 = mergedList.at(index).pos;
        Vector2D diff2 = region[goalRegion].center() - point2;
        bool reach=false;


        if(temp!=0)
        {
            switch(state)
            {
            case 0:{ //Go Behind the Object

                Vector2D space2=diff2;
                space2.setLength(300);
                rc.maxSpeed=1.2;
                rc.useNav = true;
                rc.fin_pos.loc=point2 - space2;
                rc.fin_pos.dir=diff2.dir().radian();

                object=findnearestObject(mergedShapeList,wm->ourRobot[id].pos.loc);
                if(object!=-1) ObsC=Circle2D(mergedShapeList.at(object).position,(mergedShapeList.at(object).roundedRadios+ROBOT_RADIUS+100));
                rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

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
                rc.maxSpeed=1.5;
                //if(diff2.length() > 1500) diff2.setLength(1500);
                // if(((wm->ourRobot[id].pos.loc-point2).length())>400) state=0;
                diff2.setLength(300);



//                Vector2D o2r = ( point2 - wm->ourRobot[id].pos.loc );
//                if(fabs(wm->ourRobot[id].pos.dir - o2r.dir().radian()) > AngleDeg::deg2rad(40))
//                {
//                    qDebug() << " !!!! Out OF Direction !!!! " ;
//                    state=1;//4;
//                }
                rc.fin_pos.loc=point2 + diff2;//5;
                rc.fin_pos.dir=diff2.dir().radian();

                if(((wm->ourRobot[id].pos.loc-point2).length())>400) state=0; // 60 = Robot Radius + RoundedRadius + 300
                if(((wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length())<200) state=0; // Do Not Reach The Point , The Object Lost.


//                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,10);
//                if(reach)
//                    state = 3;
            }
                break;
            case 3:{//Release alaki !!!! if Object Enters The Region , The Object Is Not Important.

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


        Vector2D dlta;
        double mrgn=300;

        if(IsInmargins(point2,mrgn))
        {
//            qDebug() << " IS IN MARGIN !!!!!!!!!";
            int side = ((point2.x-mean_x)/abs(point2.x-mean_x))*((point2.y-mean_y)/abs(point2.y-mean_y));
            if(point2.x > MAX_X-mrgn || point2.x < MIN_X+mrgn) {
                side *= ((point2.y-mean_y)/abs(point2.y-mean_y));
                dlta=Vector2D(side*10,side*(ROBOT_RADIUS+/*mergedShapeList.at(index).roundedRadios+*/50));} // side*10 ???????????
            else if(point2.y > MAX_Y-mrgn || point2.y < MIN_Y+mrgn) {
                side *=((point2.x-mean_x)/abs(point2.x-mean_x));
                dlta=Vector2D(side*(ROBOT_RADIUS+/*mergedShapeList.at(index).roundedRadios+*/50),side*10);}
            switch(statemargin)
            {
            case 0:{

                rc.fin_pos.loc=point2+dlta;

//                object=findnearestObject(mergedShapeList,wm->ourRobot[id].pos.loc);
//                if(object!=-1) ObsC=Circle2D(mergedShapeList.at(object).position,(mergedShapeList.at(object).roundedRadios+ROBOT_RADIUS));
//                rc.fin_pos.loc=AvoidtoEnterCircle(ObsC,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

//                int rad = mergedShapeList.at(index).roundedRadios+ROBOT_RADIUS;
//                Circle2D c(point2,rad);
//                rc.fin_pos.loc=AvoidtoEnterCircle(c,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

//                qDebug()<< "In Margins Pos  : ball = ( " << point2.x << ","<< point2.y << ")";
//                qDebug()<< "In Margins Pos  : delta = ( " << dlta.x << ","<< dlta.y << ")";
//                qDebug()<< "In Margins Pos  : fin_pos = ( " << rc.fin_pos.loc.x << ","<<rc.fin_pos.loc.y << ")";
//                qDebug()<< "In Margins Pos  : Robot = ( " << wm->ourRobot[id].pos.loc.x << ","<<wm->ourRobot[id].pos.loc.y << ")";
                rc.fin_pos.dir=dlta.dir().radian()-side*M_PI/2;
                reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos,rc.fin_pos,20,7);
//                            wm->ourRobot[id].pos.loc,rc.fin_pos.loc,200);
//                qDebug() << "dist To final Pos : " << (wm->ourRobot[id].pos.loc-rc.fin_pos.loc).length();
//                qDebug() << " Avoided : " << Avoided << "     reach" << reach;
                if(reach) statemargin = 1;
            }
                break;

            case 1:{
                rc.fin_pos.dir = dlta.dir().radian() - side*0.9*M_PI ;
                rc.fin_pos.loc=point2-dlta;
//                qDebug() << "Fin_POS . dir = " << AngleDeg::rad2deg(rc.fin_pos.dir) << " ROBOT . dir = " <<  AngleDeg::rad2deg(wm->ourRobot[id].pos.dir);
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

//        qDebug() << rc.fin_pos.loc.x << " -------  Y = " << rc.fin_pos.loc.y << " STATE = " << state;
//                    qDebug() << "STATE = " << state;
    }


    //rc.maxSpeed = 1.2;//rc.maxSpeed;

    //    rc.fin_pos.loc.x=rcpast.x + 0.1*(rc.fin_pos.loc.x-rcpast.x);
    //    rc.fin_pos.loc.y=rcpast.y + 0.1*(rc.fin_pos.loc.y-rcpast.y);

    //    rcpast=rc.fin_pos.loc;
//    qDebug() << " INDEX = " << index ;

    rc.maxSpeed/=1.5; // Please Do Not Clear This !!!

//    if(IsInmargins(wm->ourRobot[id].pos.loc,500)) rc.maxSpeed /= 1.5 ;
//        rc.fin_pos.loc=KeepInField(rc);

    //        qDebug() << " This Object Is For Region " << goalRegion ;

    qDebug()<<"state="<<state;

    rc.useNav=false;
    rc.isBallObs = false;
    rc.isKickObs = true;
    return rc;
}

// ============================================================================================

// ============================================================================================
void TacticTransferObject::addData()
{
    mergedShapeList.clear();
    mergedShapeList=wm->shapes4Region1;
    for(int i=0;i<wm->shapes4Region2.size();i++)
    {
        mergedShapeList.push_back(wm->shapes4Region2.at(i));
    }

    // ^^^ merging 2 List to Compare better
    Vector2D dst=wm->region1_br-wm->region1_tl;

    Vector2D cntr=wm->region1_br+wm->region1_tl;
    cntr.x /=2; cntr.y /=2 ;
    //        Size2D size1(dst.x,-dst.y);
    Rect2D rec=Rect2D::from_center(cntr,fabs(dst.x),fabs(dst.y));
    region[0]=rec;
    //        qDebug() << " WM      TOP LEFT.x = " <<wm->region2_tl.x << "    TOP LEFT . Y =  " << wm->region2_tl.y;
    //        qDebug() << " WM      BOTTOM RIGHT.x = " <<wm->region2_br.x << "    BOTTOM ROGHT . Y =  " << wm->region2_br.y;
    //        region[0].assign(wm->region1_tl,size1);//=wm->region1;
    dst=wm->region2_br-wm->region2_tl;
    cntr=wm->region2_br+wm->region2_tl;
    cntr.x /=2; cntr.y /=2 ;
    //        qDebug() << " CENTER : (" << cntr.x << "," << cntr.y << ")";
    rec=Rect2D::from_center(cntr,fabs(dst.x),fabs(dst.y));

    region[1]=rec;
    //                qDebug() << " TOP LEFT.x = " <<region[1].topLeft().x << "    TOP LEFT . Y =  " << region[1].topLeft().y;
    //                qDebug() << " BOTTOM RIGHT.x = " <<region[1].bottomRight().x << "    BOTTOM ROGHT . Y =  " << region[1].bottomRight().y;

}
// ============================================================================================
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
// ============================================================================================
void TacticTransferObject::sortData()
{
    for(int i=0;i<mergedList.size();i++)
    {
        for(int k=i+1;k<mergedList.size();k++)
        {

            if( (mergedList.at(i).pos-wm->ourRobot[id].pos.loc).length2()+(mergedList.at(i).pos-region[mergedList.at(i).goalRegion].center()).length2()
                    > (mergedList.at(k).pos-wm->ourRobot[id].pos.loc).length2()+(mergedList.at(k).pos-region[mergedList.at(k).goalRegion].center()).length2() ) mergedList.swap(i,k);

        }

    }
}

// ==================================================================================

Vector2D TacticTransferObject::KeepInField(RobotCommand rc)
{
    double margin=300;
    if(rc.fin_pos.loc.x > MAX_X - margin)
    {
        rc.fin_pos.loc = Vector2D(MAX_X,rc.fin_pos.loc.y);
//        qDebug() << " X > MAX_X !! ";
    }
    else if(rc.fin_pos.loc.x < MIN_X + margin)
    {
        rc.fin_pos.loc = Vector2D(MIN_X,rc.fin_pos.loc.y);
//        qDebug() << " X < MIN_X !! ";
    }

    if(rc.fin_pos.loc.y > MAX_Y-margin)
    {
        rc.fin_pos.loc = Vector2D(rc.fin_pos.loc.x,MAX_Y);
//        qDebug() << " Y > MAX_Y !! ";
    }
    else if(rc.fin_pos.loc.y < MIN_Y+margin)
    {
        rc.fin_pos.loc = Vector2D(rc.fin_pos.loc.x,MIN_Y);
//        qDebug() << " Y < MIN_Y !! ";
    }

    return rc.fin_pos.loc;

}
// ==================================================================================
Vector2D TacticTransferObject::AvoidtoEnterCircle(Circle2D Ci, Vector2D pnt, Vector2D finPOS)
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
//                    qDebug() << " Avoided point is : (" << finPOS.x << "," <<finPOS.y << ")  & Corrected is : ("<<
//                                p.x << "," << p.y << ")" ;
        return p;
    }
    else
    {
        return finPOS;
    }


}
// ==================================================================================
int TacticTransferObject::findnearestObject(QList<Shape> list, Vector2D Pos)
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
bool TacticTransferObject::IsInmargins(Vector2D pnt, double margin)
{

    //
    if((pnt.x > MAX_X-margin) || (pnt.x < MIN_X+margin) || (pnt.y > MAX_Y-margin) || (pnt.y < MIN_Y+margin))
    {
//        qDebug() << " IS IN MARGIN ";
        return true;
    }
    else return false;

}

// ==================================================================================


