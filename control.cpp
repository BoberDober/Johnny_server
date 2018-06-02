#include "control.h"

Control::Control(int eA, int in1, int in2, int eB, int in3, int in4, QObject *parent) : QObject(parent)
{
    typeControl = TypeControl::CONTROL;
    leftMotor = new Motor(eA, in1, in2);
    rightMotor = new Motor(eB, in3, in4);
}

void Control::move(int x, int y)
{
//    qDebug() << Q_FUNC_INFO << x << y;
    static int powerTable[6][11] =
    {
                                                //н
        {255,   204,    153,    102,    51,     0,      51,     102,    153,    204,    255},
        {51,    51,     51,     51,     51,     51,     0,      0,      0,      0,      0},
        {102,   102,    102,    102,    102,    102,    51,     0,      0,      0,      0},
        {153,   153,    153,    153,    153,    153,    102,    51,     0,      0,      0},
        {204,   204,    204,    204,    204,    204,    153,    102,    51,     0,      0},
        {255,   255,    255,    255,    255,    255,    204,    153,    102,    51,     0},
    };

    if(x == 0 && y == 0)
    {
        leftMotor->setDirection(Direction::STAY);
        rightMotor->setDirection(Direction::STAY);
    }
    else if(x < 0)
    {
        leftMotor->setDirection(Direction::BACK);
        rightMotor->setDirection(Direction::BACK);
        x = x * -1;
    }
    else if(x > 0)
    {
        leftMotor->setDirection(Direction::FORWARD);
        rightMotor->setDirection(Direction::FORWARD);
    }
    else if(x == 0)
    {
        if(y > 0)
        {
            leftMotor->setDirection(Direction::BACK);
            rightMotor->setDirection(Direction::FORWARD);
        }
        else if(y < 0)
        {
            leftMotor->setDirection(Direction::FORWARD);
            rightMotor->setDirection(Direction::BACK);
        }
    }
    leftMotor->rotate(powerTable[x][5+y]);
    rightMotor->rotate(powerTable[x][5-y]);
}
