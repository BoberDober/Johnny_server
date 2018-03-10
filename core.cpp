#include "core.h"
#include <unistd.h>

Core::Core(QObject *parent) : QObject(parent)
{
    network = new Network();
    radio = new Radio();
    timer1 = new QTimer();
    timer2 = new QTimer();

    timer1->setInterval(1000);
    timer2->setInterval(2000);

    data[0] = 0.0;
    data[1] = 0.0;
    data[2] = 0.0;
    data[3] = 0.0;

//    connect(radio, SIGNAL(dataChange(float, float)), network, SLOT(sendData(float, float)));
    connect(timer1, SIGNAL(timeout()), this, SLOT(dataRadio()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(dataBT()));
//    connect(timer, SIGNAL(timeout()), radio, SLOT(listener()));
    timer1->start();
}

void Core::johnnysLife()
{
    qDebug() << "Johnny start";
}

void Core::dataRadio()
{
    data[0] = 11.22;
    data[1] = 22.33;
    if(data[0] != 0.0 && data[1] != 0.0 && data[2] != 0.0 && data[3] != 0.0)
    {
        network->sendData(data[0], data[1], data[2], data[3]);
        data[0] = 0.0;
        data[1] = 0.0;
        data[2] = 0.0;
        data[3] = 0.0;
    }
}

void Core::dataBT()
{
    data[2] = 33.44;
    data[3] = 44.55;
    if(data[0] != 0.0 && data[1] != 0.0 && data[2] != 0.0 && data[3] != 0.0)
    {
        network->sendData(data[0], data[1], data[2], data[3]);
        data[0] = 0.0;
        data[1] = 0.0;
        data[2] = 0.0;
        data[3] = 0.0;
    }
}
