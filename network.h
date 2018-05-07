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
    QTcpSocket *clientSocket;
    QUdpSocket *clientSocketMove;
    bool connected;
public:
    explicit Network(QObject *parent = 0);
signals:
    void dataReceived(QString jsonStr);
    void dataMoveReceived(QByteArray datagram);
public slots:
    void newConnection();
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void sendData(QString data);
    void readMoveSignal();
};

#endif // NETWORK_H
