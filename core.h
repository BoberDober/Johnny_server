#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
#include <QThread>
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

    bool connectedClient;
    LedPower top_led;
    LedPower bottom_led;
    DHTData flat;
    DHTData outside;
    QThread *thread;
    bool connectedRadio1;
    bool connectedRadio2;
    TypeControl typeControl;
    bool neon;


public:
    explicit Core(QObject *parent = 0);
    void showData();
signals:
    void dataLED(int type, int r, int g, int b);
    void dataMoveReceived(QByteArray);
    void setStatus(Autopilot::Status status);
public slots:
    void dataRecieved(QString jsonStr);
    void sendData(uint8_t pipe, QByteArray data);
    void sendDataDistance(int distance);
    void dataMoveControl(QByteArray data);
    void changeConnectedRadio(int pipe, bool status);
    void changeConnectedClient(bool connected);
};

#endif // CORE_H
