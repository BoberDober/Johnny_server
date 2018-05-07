#ifndef CONTROL_H
#define CONTROL_H

#include "motor.h"
#include "autopilot.h"
#include <QObject>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QThread>

enum TypeControl {CONTROL, AUTONOMUS};

class Control : public QObject
{
    Q_OBJECT
//    Autopilot *autopitol;
public:
    explicit Control(int eA, int in1, int in2, int eB, int in3, int in4, QObject *parent = 0);
    TypeControl typeControl;
//    QThread *thread;
    Motor *leftMotor;
    Motor *rightMotor;
signals:
    void changeDistance(int distance);
public slots:
    void move(int x, int y);
};

#endif // CONTROL_H
