#include "network.h"


Network::Network(QObject *parent) : QObject(parent)
{
    connected = false;
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    clientSocket = new QTcpSocket(this);

    if (!server->listen(QHostAddress::Any, 8000))
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(server->errorString());
    else
        qDebug() << "Server START";

    clientSocketMove = new QUdpSocket();
    if (!clientSocketMove->bind(QHostAddress::Any, 8001))
        qDebug() << "Server move FAILED";
    else
        qDebug() << "Server move START";
    connect(clientSocketMove, SIGNAL(readyRead()), this, SLOT(readMoveSignal()));
}

void Network::newConnection()
{
    if(connected)
        qDebug() << "CONNECTION IS BUSY";
    else
    {
        qDebug() << "NEW CONNECTION";
        clientSocket = server->nextPendingConnection();
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(clientSocket, SIGNAL(connected()), this, SLOT(onConnected()));
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        qDebug() << clientSocket->peerAddress() << clientSocket->peerPort();
        connected = true;
        changeConnected(connected);
    }
}

void Network::onReadyRead()
{
//    qDebug() << "NEW DATA";
    QByteArray datagram;
    QString jsonStr;
    datagram = clientSocket->readAll();
//    qDebug() << datagram.size();
    QDataStream ds(&datagram, QIODevice::ReadOnly);
    ds >> jsonStr;
    emit dataReceived(jsonStr);
}

void Network::onConnected()
{
    qDebug() << "CONNECTED";
    connected = true;
    changeConnected(connected);
}

void Network::onDisconnected()
{
    qDebug() << "DISCONNECTED";
    connected = false;
    changeConnected(connected);
}

void Network::sendData(QString data)
{
    if(connected)
    {
        QByteArray arr;
        QDataStream ds(&arr, QIODevice::WriteOnly);
        ds << data;
        clientSocket->write(arr);
    }
}

void Network::readMoveSignal()
{
//    qDebug() << "MOVE DATA";
    QByteArray datagram;
    datagram.resize(clientSocketMove->pendingDatagramSize());
    QHostAddress *address = new QHostAddress();
    clientSocketMove->readDatagram(datagram.data(), datagram.size(), address);
    emit dataMoveReceived(datagram);
}
