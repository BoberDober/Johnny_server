#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>

enum Status {STOP = 0, START};

class Autopilot : public QObject
{
    Q_OBJECT
    Status status;
public:
    explicit Autopilot(QObject *parent = 0);
    void setStatus(Status _status);
signals:
    void dataMoveReceived(int x, int y);
public slots:
    void setDistance(int distance);
};

#endif // AUTOPILOT_H
