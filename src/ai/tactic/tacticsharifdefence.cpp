    #include "tacticsharifdefence.h"

    TacticSharifDefence::TacticSharifDefence(WorldModel *worldmodel, QObject *parent) :
        Tactic("TacticSharifDefence", worldmodel, parent)
    {
        circularBorder.assign(Vector2D(1500,0),1700/2);
        circularBorderOut.assign(Vector2D(1500,0),2100/2);// a circle may use to push balls with some risks
        circularMid.assign(Vector2D(1500,0),720); // a circle which is between holes and border circle
        hole1.assign(Vector2D(1500,1700/4),250/2);
        hole2.assign(Vector2D(1500,-1700/4),250/2);
        //        addHolePoints();
        hole1_Offset.assign(Vector2D(1500,1700/4),650/2);
        hole2_Offset.assign(Vector2D(1500,-1700/4),650/2);
        Vector2D Cdist = (hole1.center() - circularBorder.center());
        double deltaAngle=1.1*asin(hole1.radius()/(Cdist.length())); // 1.1 is safety factor
        Uangle1=Cdist.dir().radian() + deltaAngle ;
        Dangle1=Uangle1 - 2*deltaAngle;
        Cdist = (hole2.center() - circularBorder.center());
        Uangle2=Cdist.dir().radian() + deltaAngle ;
        Dangle2=Uangle2 - 2*deltaAngle;
        firstInit=true;


        loop=0;


    }

    RobotCommand TacticSharifDefence::getCommand()
    {
        RobotCommand rc;
        if(!wm->ourRobot[id].isValid) return rc;
        rc.fin_pos.loc=circularBorder.center(); //
        rc.maxSpeed = 1.4;
        rc.useNav=true;
        float minVel=0.2;
        towardH1=false;
        towardH2=false;
        InHole=false;
        Avoided=false;
        bool reach;
        oppIsInField=wm->theirRobot.IsValid;//wm->ourRobot[7].isValid;
        Opp=wm->theirRobot.position;//wm->ourRobot[7].pos.loc;
        oppIsKhoraak=circularBorder.contains(Opp);

        addData();

        sortData();
        index=-1;

        AllIn=true;
        AnyIn=false;

        //qDebug()<< " U 1 =" << Uangle1 << "   D 1 =" << Dangle1 << " /////// U2 = " << Uangle2 << "   D2 = " << Dangle2 ;
        //qDebug() << " ANGLE     = " << (circularBorder.center()-balls.at(0)).dir().radian();

        for(int i=0;i<balls.size();i++) // if Any ball is Toward each hole , catch it
        {
            if(!hole1_Offset.contains(balls.at(i)->pos.loc) && !hole2_Offset.contains(balls.at(i)->pos.loc))
            {
                //c2o = new Segment2D(circularBorderOut.center() , balls.at(i) );
                //temp=0;
                IsInside   = circularBorder  .contains(balls.at(i)->pos.loc);
                Accessible = circularBorderOut.contains(balls.at(i)->pos.loc);

                if(!IsInside) AllIn = false;
                else if(IsInside) AnyIn = true;

                vel=balls.at(i)->vel.loc;
                ballVel = new Line2D(point , vel.dir().degree());
                towardH1 = hole1.HasIntersection(*ballVel,1.5);
                towardH2 = hole2.HasIntersection(*ballVel,1.5);

                if((towardH1 || towardH2) && vel.length() > minVel )
                {
                    index=i;
                    qDebug() << " Ball # " << i << " Is Toward a Hole ";
                    break;

                }
            }

            //            else
            //            {
            //                index=i;
            //                break;
            //            }

        }

        if(index !=-1)
        {

            //            qDebug() << "ball's velocity = " << vel.length() ;
            //            point = findnearest2circle(); //balls.at(index);
            point=wm->balls.at(index)->pos.loc;



            if(towardH1 && !towardH2 && (vel.length() > minVel) )
            {
                state=2; // just to Detect for Qdebug()
                qDebug() << " TOWARD HOLE # 1";
                Vector2D nrstpnt = (point - hole1.center());
                rc.fin_pos.dir = nrstpnt.dir().radian() ;
                nrstpnt=nrstpnt.setLength(hole1.radius()+ROBOT_RADIUS + 50);//circularMid.radius());//
                nrstpnt=nrstpnt + hole1.center();//findnearesthole(point) ;//
                rc.fin_pos.loc=nrstpnt;
            }

            else if(towardH2 && !towardH1 && (vel.length() > minVel) )
            {
                state=3; // just to Detect for Qdebug()
                qDebug() << " TOWARD HOLE # 2";
                Vector2D nrstpnt = (point - hole2.center());
                rc.fin_pos.dir = nrstpnt.dir().radian() ;
                nrstpnt=nrstpnt.setLength(hole2.radius()+ROBOT_RADIUS + 50);//circularMid.radius());//
                nrstpnt=nrstpnt + hole2.center();//findnearesthole(point) ;//
                rc.fin_pos.loc=nrstpnt;
            }

            else if(towardH1 && towardH2 && (vel.length() > minVel) )
            {
                state=4; // just to Detect for Qdebug()
                Vector2D distH1 = (point - hole1.center());
                Vector2D distH2 = (point - hole2.center());
                if(distH1.length2() < distH2.length2())
                {
                    distH1.setLength(hole1.radius() + ROBOT_RADIUS + 50);
                    rc.fin_pos.loc = hole1.center() + distH1 ;
                    rc.fin_pos.dir = distH1.dir().radian();
                }
                else
                {
                    distH2.setLength(hole2.radius() + ROBOT_RADIUS + 50);
                    rc.fin_pos.loc = hole2.center() + distH2 ;
                    rc.fin_pos.dir = distH2.dir().radian();
                }
            }

            //Vector2D diff2 = nrstpnt - point2;// wm->ourRobot[id].pos.loc ;
            //bool reach=false;
        }
        else
        {
            if(balls.size() > 0 )
            {

                point=balls.at(0)->pos.loc;

                if(!AnyIn)
                {
                    //                        else
                    //
                    if(oppIsInField)
                    {
                        nearest2Opp();
                        point=balls.at(index)->pos.loc;
                    }
                    else point=findnearest2circle();
                    //                        }
                    //                        state=1;
                    //qDebug() << " ANYIN is FFFFFFFFFFFFFFALSE";

                    //qDebug() << " POINT . X = "  <<  point.x << "          POINT . Y =" << point.y;
                    Vector2D nrstpnt = (point - circularBorderOut.center());//findnearesthole(point));//
                    rc.fin_pos.dir = nrstpnt.dir().radian() ;
                    nrstpnt=nrstpnt.setLength(circularMid.radius()+40);//300);//
                    //Vector2D diff = nrstpnt;
                    nrstpnt=nrstpnt + circularBorder.center();//findnearesthole(point) ;//
                    //qDebug() << " X  = " << nrstpnt.x << "   Y =  " << nrstpnt.y;
                    rc.fin_pos.loc=nrstpnt;//goOncircle2point(nrstpnt);
                    rc.useNav=true;

                }

                else if( AnyIn)
                {
                    state=10;
                    //qDebug() << " ANYIN is TRUE ";
                    //                        point = balls.at(index)->pos.loc;
                    Vector2D nrstpnt = circularBorder.center()-point;//findnearesthole(point)-point);
                    Line2D ball2center(point,circularBorder.center());
                    //                        double ang=nrstpnt.dir().radian();
                    //if (!(((ang < Uangle1 && ang > Dangle1) || (ang < Uangle2 && ang > Dangle2) ) && (nrstpnt.length2() < (circularBorder.center()-hole1.center()).length2())))//&& ang < Dangle1 && ang < Dangle2)
                    if((hole1.HasIntersection(ball2center) || hole2.HasIntersection(ball2center)) && (nrstpnt.length2() < 1.1*(circularBorder.center()-hole1.center()).length2()) )
                    {
                        state=101;
                        qDebug() << " NAKON TOO SOoooooooooooooooooooooORAAKH !!!";
                        nrstpnt.setDir(nrstpnt.dir()+90);//M_PI/2);
                        nrstpnt.setLength(150);
                        rc.fin_pos.dir=nrstpnt.dir().radian()+M_PI;
                        nrstpnt=point+nrstpnt;
                        rc.fin_pos.loc=nrstpnt;

//                        Circle2D C(point,ROBOT_RADIUS+100);
//                        rc.fin_pos.loc=AvoidtoEnterCircle(C,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);
                        rc.maxSpeed=1;

                        bool reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,70);

                        if(reach)
                        {
                            nrstpnt = point - nrstpnt;
                            nrstpnt.setLength(150);
                            rc.fin_pos.dir=nrstpnt.dir().radian();//+M_PI;
                            nrstpnt = point + nrstpnt;
                            rc.fin_pos.loc=nrstpnt;
                            //                                rc.useNav=false;
                        }
                        //rc.fin_pos.loc=circularBorder.center();
                    }
                    else
                    {
                        state=102;
                        //qDebug() << ang ;
                        qDebug() << " biroon SOORAAKH !!!";

                        nrstpnt.setDir(nrstpnt.dir());//+M_PI/2);
                        nrstpnt.setLength(150);
                        rc.fin_pos.dir=nrstpnt.dir().radian()+M_PI;
                        nrstpnt=point+nrstpnt;
                        rc.fin_pos.loc=nrstpnt;

//                        Circle2D C(point,ROBOT_RADIUS+100);
//                        rc.fin_pos.loc=AvoidtoEnterCircle(C,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);
                        rc.maxSpeed=1;
                        //rc.fin_pos.dir=nrstpnt.dir().radian();//+M_PI;
                        bool reach=wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc,rc.fin_pos.loc,70);
                        if(reach)
                        {
                            nrstpnt = point - nrstpnt;
                            nrstpnt.setLength(150);
                            rc.fin_pos.dir=nrstpnt.dir().radian();//+M_PI;
                            nrstpnt = point + nrstpnt;
                            rc.fin_pos.loc=nrstpnt;
                            //rc.fin_pos.dir=nrstpnt.dir().radian()+M_PI;
                            //                                rc.useNav=false;
                        }
                    }


                }
            }

            //                    point=findnearest2circle();




        }

        qDebug()<< "INDEX = " << index << "fin_pos.x  " << rc.fin_pos.loc.x << "  Y  "<<rc.fin_pos.loc.y<< " ------------------------------ STATE = " << state << "    STATE 2 =" << state2;
        qDebug() << " S T A T E  =  " << state;
        qDebug() << " Opp Is In Field : " << oppIsInField << "    AllIn  : " << AllIn << "    Any in :  " << AnyIn ;

        rc.fin_pos.loc=AvoidtoEnterCircle(hole1_Offset,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

        rc.fin_pos.loc=AvoidtoEnterCircle(hole2_Offset,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);

        qDebug()<<" Detected Ball  Is In = (" << point.x << "," << point.y << ")";
        //        if(reach) rc.fin_pos.loc=AvoidtoEnterCircle(hole2_Offset,hole2_V,wm->ourRobot[id].pos.loc,rc.fin_pos.loc);


        //        if(InHole) rc.maxSpeed=1;
        //        rc.useNav=true;
        if(Avoided) rc.maxSpeed *= 0.5;
        if(oppIsKhoraak) rc.fin_pos.loc=Opp;
        //        rc.isBallObs = true;//false;
        //        rc.isKickObs = true;

        //        rc.fin_pos.loc=Vector2D(200,0);
        return rc;
    }
    // ==================================================================================
    void TacticSharifDefence::addData()
    {

        balls.clear();
        //balls.insert(0,&wm->ball);//wm->ourRobot[0].pos.loc);
        //        for(int i=0;i<wm->numberOfBalls;i++)
        //        {
        //            //        Aball[i].pos=wm->balls[i].pos;
        //            this->balls.push_back(wm->ball*[i]);

        //        }
        balls=wm->balls;
        //balls.insert(1,&wm->ourRobot[1]);
        //balls.insert(2,wm->ourRobot[2].pos.loc);

//        oppIsInField = wm->oppRobot[7].isValid;
//        OppositeRobot = wm->oppRobot[7].pos.loc;

        //        firstInit=false;
        //        loop++;
    }
    // ==================================================================================
    //    void TacticSharifDefence::addHolePoints()
    //    {
    //        double offset=200+ROBOT_RADIUS;
    //        Vector2D vec;//=Vector2D(0,0);
    //        for(int i=0;i<8;i++)
    //        {
    //            //            vec.assign(
    //            vec.assign(((hole1.radius()+offset)*cos(i*M_PI/4)),((hole1.radius()+offset)*sin((i/4)*M_PI)));
    //            //            vec.y=(hole1.radius()+offset)*sin(double(i/4)*M_PI);
    //            //            vec.setLength(hole1.radius()+offset);
    //            //            vec.setDir(double(i/4)*M_PI);
    //            hole1_V[i]=hole1.center() + vec;
    //            hole2_V[i]=hole2.center() + vec;
    //        }

    //    }

    // ==================================================================================
    //Vector2D TacticSharifDefence::findnearest(Vector2D input)
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
    void TacticSharifDefence::sortData()
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
    void TacticSharifDefence::addVel()
    {
        for(int j=0;j<wm->balls.size();j++)
        {
            Vector2D diffrence = (lastBalls.at(j)->pos.loc-balls.at(j)->pos.loc);
            if(diffrence.length2() <  6000) //maxVel = 5 m/s
            {
                balls.at(j)->vel.loc=diffrence;
                balls.at(j)->vel.dir=diffrence.dir().radian();
            }
            else balls.at(j)->vel.loc=Vector2D(0,0);
        }
    }
    // ==================================================================================
    void TacticSharifDefence::goOncircle2point(Vector2D pnt)
    {

    }
    // ==================================================================================
    void TacticSharifDefence::nearest2Opp()
    {
        double dist1 = 1000000000000000;
        for(int j=0;j<balls.size();j++)
        {
            if(((balls.at(j)->pos.loc-Opp).length2()) < dist1 )
            {
                dist1=(balls.at(j)->pos.loc-Opp).length2();
                index=j;
            }
            //        qDebug() << " DIST  = " << dist1 ;
        }
    }

    // =================================================================================
    Vector2D TacticSharifDefence::findnearest2circle()
    {
        int tmp=0;
        for(int k=0;k<balls.size();k++)
        {
            if( (balls.at(k)->pos.loc-circularBorder.center()).length2() < (balls.at(tmp)->pos.loc-circularBorder.center()).length2()) tmp=k;
        }
        return balls.at(tmp)->pos.loc;
    }
    // ==================================================================================
    Vector2D TacticSharifDefence::findnearesthole(Vector2D pnt)
    {
        if( (pnt-hole1.center()).length2() < (pnt-hole2.center()).length2() ) return hole1.center();
        else return hole2.center();
    }
    // ==================================================================================
    Vector2D TacticSharifDefence::AvoidtoEnterCircle(Circle2D Ci,Vector2D pnt , Vector2D finPOS)
    {
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
            int div=int(Ci.radius()/70);
            //        qDebug() << "div = " << div ;
            angle=angle/=min(div,3);
            angle=c2r.dir() - angle;
            c2r.setDir(angle);// c2f.dir()));
            Vector2D p = Ci.center() + c2r ;
            //            Avoided=true;
            qDebug() << " Avoided point is : (" << finPOS.x << "," <<finPOS.y << ")  & Corrected is : ("<<
                        p.x << "," << p.y << ")" ;

            Avoided=true;
            return p;
        }
        else
        {
            return finPOS;

        }

    }
    // ==================================================================================
    // ==================================================================================
