#include "autopilot.h"

Autopilot::Autopilot(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<Autopilot::Status>("Autopilot::Status");
    status = STOP;
    distance = 0;
    timer = new QTimer(this);
    timer->setInterval(200);
    connect(timer, SIGNAL(timeout()),this, SLOT(loop()));
}

void Autopilot::setStatus(Autopilot::Status _status)
{
    status = _status;
    switch (_status) {
    case Status::STOP:
        timer->stop();
        break;
    case Status::START:
        timer->start();
        break;
    default:
        break;
    }
}

void Autopilot::loop()
{
    int x = 0;
    int y = 0;
    if(distance > 55)
    {
        x = 1;
        y = 0;
    }
    else
    {
        x = 0;
        y = 2;
    }
//    qDebug() << "distance - " << distance;
    emit dataMoveReceived(x, y);
}

void Autopilot::setDistance(int _distance)
{
    distance = _distance;
}
