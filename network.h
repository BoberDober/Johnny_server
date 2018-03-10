#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>

class Network : public QObject
{
    Q_OBJECT
    QTcpServer *server;
    QTcpSocket* clientSocket;
//    QHostAddress ip_client;
//    quint16 port_client;
    bool connected;
public:
    explicit Network(QObject *parent = 0);
signals:

public slots:
    void newConnection();

    void onReadyRead();
    void onConnected();
    void onDisconnected();

    void sendData(float flatTemperature, float flatHumidity, float outsideTemperature, float outsideHumidity);
};

#endif // NETWORK_H
