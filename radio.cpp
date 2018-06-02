#include "radio.h"

#include <QDataStream>

Radio::Radio(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    timer_connected1 = new QTimer();
    timer_connected2 = new QTimer();
    connected1 = false;
    connected2 = false;
    typewrite = NONE;
#ifdef PI
//    radio = new RF24(22,0);
//    radio->begin();
//    radio->setRetries(15,15);
//    radio->setChannel(5);
//    radio->setDataRate     (RF24_1MBPS);
//    radio->setPALevel(RF24_PA_HIGH);
//    radio->printDetails();
//    radio->openWritingPipe(0xAABBCCDD11LL);
//    radio->openReadingPipe(1,0xAABBCCDD22LL);
//    radio->stopListening();
//    radio->startListening();


    radio = new RF24(22,0);
    radio->begin();
    radio->setRetries(15,15);
    radio->printDetails();
    radio->openReadingPipe(1,pipes[0]);
    radio->openReadingPipe(2,pipes[2]);
    radio->openWritingPipe(pipes[1]);
    radio->startListening();

#endif
    connect(timer,SIGNAL(timeout()), this, SLOT(listener()));
    connect(timer_connected1,SIGNAL(timeout()), this, SLOT(connectedRadio1()));
    connect(timer_connected2,SIGNAL(timeout()), this, SLOT(connectedRadio2()));

    valueLight[0] = 0;
    valueLight[1] = 0;
    valueLight[2] = 0;
    valueLight[3] = 0;
    timer->setInterval(2000);
    timer_connected1->setInterval(5000);
    timer_connected2->setInterval(5000);
    timer->start();
    timer_connected1->start();
    timer_connected2->start();
}

void Radio::sendData(Typewrite type, QByteArray data) // передавать char
{
    QDataStream datastream(&data, QIODevice::ReadOnly);
    switch (type) {
    case LED:
        int buffer[4];
        datastream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
//        qDebug() << "STATUS - " << buffer[0] << buffer[1] << buffer[2] << buffer[3];
        #ifdef PI
            radio->stopListening();
            radio->openWritingPipe(pipes[3]);
            radio->write(&buffer, sizeof(buffer));
//            if (!radio->write(&buffer, sizeof(buffer)))
//                qDebug() << "---FAILED SEND DATA---";
//            else
//                qDebug() << "---SUCCSESS SEND DATA---";
            radio->startListening();
        #endif
        break;
    case NEON:
        bool status;
        datastream >> status;
//        qDebug() << "STATUS - " << status;
        #ifdef PI
            radio->stopListening();
            radio->openWritingPipe(pipes[1]);
            radio->write(&status, sizeof(status));
//            if (!radio->write(&status, sizeof(status)))
//                qDebug() << "---FAILED SEND DATA---";
//            else
//                qDebug() << "---SUCCSESS SEND DATA---";
            radio->startListening();
        #endif
        break;
    default:
        break;
    }
}

void Radio::connectedRadio1()
{
    connected1 = false;
    emit connectedRadio(1, connected1);
}

void Radio::connectedRadio2()
{
    connected2 = false;
    emit connectedRadio(2, connected2);
}

void Radio::listener()
{
#ifdef PI
    uint8_t pipe;
    radio->startListening();
    float data[2];
    if(radio->available(&pipe))
    {
        QByteArray array;
        QDataStream datastream(&array, QIODevice::WriteOnly);
        radio->read(data, sizeof(data));
//        qDebug() << data[0] << " " << data[1];
        datastream << data[0] << data[1];
        if(pipe == 1)
        {
            timer_connected1->start();
            connected1 = true;
//            if(!connected1)
                emit connectedRadio(1, connected1);
        }
        else if(pipe == 2)
        {
            timer_connected2->start();
            connected2 = true;
//            if(!connected2)
                emit connectedRadio(2, connected2);
        }
        emit dataChange(pipe, array);
    }
//    radio->stopListening();
#endif
}
