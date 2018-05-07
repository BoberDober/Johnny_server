#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>
#include <QDebug>

enum Direction {STAY = 0, FORWARD, BACK};

class Motor : public QObject
{
    Q_OBJECT
    int enA;
    int in1;
    int in2;
    Direction direction;
public:
    explicit Motor(int _enA, int _in1, int in2, QObject *parent = 0);
    void rotate(int pwm);
    void setDirection(Direction _direction);
signals:

public slots:
};

#endif // MOTOR_H
