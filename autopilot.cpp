#include "autopilot.h"

Autopilot::Autopilot(QObject *parent) : QObject(parent)
{
    status = STOP;
}

void Autopilot::setStatus(Status _status)
{
    status = _status;
}

void Autopilot::setDistance(int distance)
{
    if(status == STOP)
        qDebug() << "STOP";
    else if(status == START)
    {
        int x = 0;
        int y = 0;
        if(distance > 25)
        {
            x = 20;
            y = 0;
        }
        else
        {
            x = 0;
            y = 20;
        }
        qDebug() << "distance - " << distance;
        emit dataMoveReceived(x, y);
    }
}

