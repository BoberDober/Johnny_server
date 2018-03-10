#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTimer>
#include "network.h"
#include "radio.h"

class Core : public QObject
{
    Q_OBJECT
    Network *network;
    Radio *radio;
    QTimer *timer1;
    QTimer *timer2;
    float data[4];
public:
    explicit Core(QObject *parent = 0);
    void johnnysLife();
signals:
    void changeData(float d1, float d2);
public slots:
    void dataRadio();
    void dataBT();

};

#endif // CORE_H
