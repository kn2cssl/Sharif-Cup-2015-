#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <QList>
#include <QDebug>
#include "constants.h"
#include "base.h"
#include "ball.h"
#include "robot.h"
#include "agent.h"
#include "geom.h"
#include "game_state.h"
#include "ref_protocol.h"
#include "knowledge.h"
#include "shape.h"

class WorldModel
{
public:
    explicit WorldModel(OutputBuffer *outputBuffer);
    double time;
    Ball ball;
    Agent ourRobot[PLAYERS_MAX_NUM];
    Robot oppRobot[PLAYERS_MAX_NUM];
    GameStateType gs;
    GameStateType gs_last;
    GameState cmgs; //referee
    int ref_goalie_our;
    int ref_goalie_opp;
    bool isSim;
    Knowledge *kn;

    static double var[10];
    // 0: kick activation dist
    // 1: kick activation angle
    // 2: kick precision angle
    // 3: prediction sliding factor

    //--------SharifCup 2014---------
    int mission;
    int numberOfShapes;
    Agent *myRobot;
    Shape theirRobot;
    QList<Shape> Chasbideh;
        //-------First Mission----------
        Vector2D region1_tl;
        Vector2D region1_br;
        Vector2D region2_tl;
        Vector2D region2_br;
        QList<Shape> shapes4Region1;
        QList<Shape> shapes4Region2;

        //-----------------------------

        //-------Second Mission--------
        QList<Segment2D> borders;
        QList<Shape> positiveShapes;
        QList<Shape> negativeShapes;
        //-----------------------------

        //-------Third Mission---------
        bool isAttacker;
        //int numberOfBalls;
        QList<Ball*> balls;
        //QList<Position> ballsWithoutSpeed;
        Circle2D circularBorder;
        Circle2D goal1;
        Circle2D goal2;
        //-----------------------------

    Vector2D endPoint;
    QList<Shape> obstacles;
    //----------------------------

    void clean();
};

#endif // WORLDMODEL_H
