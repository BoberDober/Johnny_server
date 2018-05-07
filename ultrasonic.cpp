#include "ultrasonic.h"
#ifdef PI
    #include <wiringPi.h>
#endif

Ultrasonic::Ultrasonic(int _pinTRIG, int _pinECHO, QObject *parent) : QObject(parent)
{
    pinTRIG = _pinTRIG;
    pinECHO = _pinECHO;
    timer = new QTimer();
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()),this, SLOT(getDistance()));
    timer->start();
#ifdef PI
    wiringPiSetup();
    pinMode(pinTRIG, OUTPUT);
    pinMode(pinECHO, INPUT);
    digitalWrite(pinTRIG, LOW);
#endif
}

void Ultrasonic::getDistance()
{
    int distance = 0;
#ifdef PI

    volatile long startTimeUsec;
    volatile long endTimeUsec;
    long travelTimeUsec;
    long now;

    digitalWrite(pinTRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTRIG, LOW);
    now=micros();
    while (digitalRead(pinECHO) == LOW && micros());
    {
        startTimeUsec = micros();
        while ( digitalRead(pinECHO) == HIGH );
        endTimeUsec = micros();
    }
    travelTimeUsec = endTimeUsec - startTimeUsec;
    distance = 100*((travelTimeUsec/1000000.0)*340.29)/2;

#endif
    emit distanceChange(distance);
}

////Send trig pulse
//digitalWrite(pinTRIG, HIGH);
//delayMicroseconds(20);
//digitalWrite(pinTRIG, LOW);

////Wait for echo start
//while(digitalRead(pinECHO) == LOW);

////Wait for echo end
//long startTime = micros();
//while(digitalRead(pinECHO) == HIGH);
//long travelTime = micros() - startTime;

////Get distance in cm
//distance = travelTime / 58;
