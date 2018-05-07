#include "motor.h"
#ifdef PI
    #include <wiringPi.h>
    #include <softPwm.h>
#endif

Motor::Motor(int _enA, int _in1, int _in2, QObject *parent) : QObject(parent)
{
    enA = _enA;
    in1 = _in1;
    in2 = _in2;
    direction = STAY;
#ifdef PI
    wiringPiSetup();
    softPwmCreate(enA, 0, 255);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);

    digitalWrite(enA, LOW);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
#endif
}

void Motor::rotate(int pwm)
{
    #ifdef PI
        softPwmWrite(enA, pwm);
    #endif
    qDebug() << pwm;
}

void Motor::setDirection(Direction _direction)
{
    direction = _direction;
    if(direction == FORWARD)
    {
        #ifdef PI
            digitalWrite(in1, HIGH) ;
            digitalWrite(in2, LOW) ;
        #endif
//        qDebug() << "FORWARD";
    }
    else if(direction == BACK)
    {
        #ifdef PI
            digitalWrite(in1, LOW) ;
            digitalWrite(in2, HIGH) ;
        #endif
//        qDebug() << "BACK";
    }
    else if(direction == STAY)
    {
        #ifdef PI
            digitalWrite(in1, HIGH) ;
            digitalWrite(in2, LOW) ;
            softPwmWrite(enA, 0);
        #endif
//        qDebug() << "STAY";
    }
}
