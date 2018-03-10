#include "radio.h"

Radio::Radio(QObject *parent) : QObject(parent)
{
//    radio = new RF24(22,0);
//    radio->begin();
//    radio->setRetries(15,15);
//    radio->printDetails();
//    radio->openReadingPipe(1,pipes[0]);
//    radio->startListening();
    qDebug() << "GettingStarted" << endl;
}

void Radio::listener()
{
//    qDebug() << Q_FUNC_INFO;
//    if(radio->available())
//    {
//        float data[2];
//        radio->read(data, sizeof(data));
//        qDebug() << "DATA" << data[0] << data[1] << endl;
//        emit dataChange(data[0], data[1]);
//    }
//    else
//    {
//        qDebug() << "NO DATA";
//    }
}
