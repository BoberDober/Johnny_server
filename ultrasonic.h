#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <QObject>
#include <QTimer>

class Ultrasonic : public QObject
{
    Q_OBJECT
    int pinTRIG;
    int pinECHO;
    QTimer *timer;
public:
    explicit Ultrasonic(int _pinTRIG, int _pinECHO, QObject *parent = 0);
signals:
    void distanceChange(int distance);
public slots:
    void getDistance();
};

#endif // ULTRASONIC_H
