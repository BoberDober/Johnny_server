#ifndef RADIO_H
#define RADIO_H

#include <QObject>
#include <QDebug>
//#include <RF24/RF24.h>

class Radio : public QObject
{
    Q_OBJECT
//    RF24 *radio;
    const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

public:
    explicit Radio(QObject *parent = 0);
signals:
    void dataChange(float temperature, float humidity);
public slots:
    void listener();
};

#endif // RADIO_H
