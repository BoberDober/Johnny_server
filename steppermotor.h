#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <QObject>

class StepperMotor : public QObject
{
    Q_OBJECT
    int inputPins[4];
public:
    explicit StepperMotor(int pinA, int pinB, int pinC, int pinD, QObject *parent = 0);
    void angleRotation(float angle);
    void step(int noOfSteps);
    void writeSequence(const unsigned int sequenceNo);
signals:

public slots:
};

#endif // STEPPERMOTOR_H
