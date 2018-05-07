#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
#include "network.h"
#include "radio.h"
#include "control.h"
#include "ultrasonic.h"
#include "autopilot.h"

struct LedPower {
    int redPower;
    int greenPower;
    int bluePower;
};

struct DHTData {
    float temperature;
    float humidity;
};

class Core: public QObject
{
    Q_OBJECT
    Network *network;
    Radio *radio;
    Control *control;
    Ultrasonic *ultrasonic;
    Autopilot *autopitol;

    LedPower top_led;
    LedPower bottom_led;
    DHTData flat;
    DHTData outside;

    TypeControl typeControl;
    bool neon;


public:
    explicit Core(QObject *parent = 0);
signals:
    void dataLED(int type, int r, int g, int b);
    void dataMoveReceived(QByteArray);
public slots:
    void dataRecieved(QString jsonStr);
    void sendData(uint8_t pipe, QByteArray data);
    void dataMoveControl(QByteArray data);
};

#endif // CORE_H
