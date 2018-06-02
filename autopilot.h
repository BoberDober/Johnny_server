#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QTimer>

class Autopilot : public QObject
{
    Q_OBJECT
//    Q_ENUM(Status)
public:
    explicit Autopilot(QObject *parent = 0);
    enum Status {STOP = 0, START};
signals:
    void dataMoveReceived(int x, int y);
public slots:
    void setDistance(int _distance);
    void setStatus(Autopilot::Status _status);
    void loop();

    // QObject interface
protected:
//    void timerEvent(QTimerEvent *event);
private:
    int distance;
    Status status;
    QTimer *timer;
};

#endif // AUTOPILOT_H
