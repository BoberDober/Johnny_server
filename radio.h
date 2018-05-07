#ifndef RADIO_H
#define RADIO_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QDataStream>
#ifdef PI
    #include <RF24/RF24.h>
#endif

enum Typewrite {NONE = 0, LED, NEON};

//0xAABBCC2211LL - модуль 2 read
//0xAABBCC2200LL - модуль 2 write

class Radio : public QObject
{
    Q_OBJECT
#ifdef PI
    RF24 *radio;
#endif
    QTimer *timer;
    Typewrite typewrite;
//    const uint64_t pipes[2] = { 0xAABBCC4400LL, 0xAABBCC1100LL};
//    const uint64_t pipes[4] = { 0xAABBCC1111LL, 0xAABBCC1100LL, 0xAABBCC2211LL, 0xAABBCC2200LL};
    const uint64_t pipes[4] = { 0xAABBCCDDF0LL, 0xAABBCCDDF1LL, 0xAABBCCDDF2LL, 0xAABBCCDDF3LL};
    int valueLight[4];
public:
    explicit Radio(QObject *parent = 0);
signals:
    void dataChange(uint8_t, QByteArray);
public slots:
    void sendData(Typewrite type, QByteArray data);
    void listener();
};

#endif // RADIO_H
