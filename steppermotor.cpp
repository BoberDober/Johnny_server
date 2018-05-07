#include "steppermotor.h"
#define LOW 0
#define HIGH 1

#ifdef PI
    #include <wiringPi.h>
#endif

StepperMotor::StepperMotor(int pinA, int pinB, int pinC, int pinD, QObject *parent) : QObject(parent)
{
    inputPins[0] = pinA;
    inputPins[1] = pinB;
    inputPins[2] = pinC;
    inputPins[3] = pinD;
    #ifdef PI
        for (int i = 0; i < 4; i++)
            pinMode(inputPins[i], OUTPUT);
    #endif
}

void StepperMotor::angleRotation(float angle)
{
    int steps = (int)(512*8*angle)/360;
    step(steps);
}

void StepperMotor::step(int noOfSteps)
{
    if (noOfSteps > 0)
    {
        for (int currentStep = noOfSteps; currentStep > 0; currentStep--)
        {
            int currentSequenceNo = currentStep % 8;
            writeSequence(currentSequenceNo);
        }
    }
    else
    {
        for (int currentStep = 0; currentStep < abs(noOfSteps); currentStep++)
        {
            int currentSequenceNo = currentStep % 8;
            writeSequence(currentSequenceNo);
        }
    }
}

void StepperMotor::writeSequence(const unsigned int sequenceNo)
{
    static bool HALF_STEP_MOTOR_SEQUENCE[][4] = {
            { LOW,  LOW,  LOW,  HIGH },
            { LOW,  LOW,  HIGH, HIGH },
            { LOW,  LOW,  HIGH, LOW },
            { LOW,  HIGH, HIGH, LOW },
            { LOW,  HIGH, LOW,  LOW },
            { HIGH, HIGH, LOW,  LOW },
            { HIGH, LOW,  LOW,  LOW },
            { HIGH, LOW,  LOW,  HIGH }
    };

#ifdef PI
    for (int i = 0; i < 4; i++)
        digitalWrite(inputPins[i], HALF_STEP_MOTOR_SEQUENCE[sequenceNo][i]);
//	delay(stepDuration);
    for (int i = 0; i < 4; i++) {
        digitalWrite(inputPins[i], LOW);
    }
#endif
}
