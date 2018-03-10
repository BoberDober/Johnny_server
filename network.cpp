#include "network.h"


Network::Network(QObject *parent) : QObject(parent)
{
    connected = false;
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if (!server->listen(QHostAddress::Any, 8000))
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(server->errorString());
    else
        qDebug() << QString::fromUtf8("Server START");
}

void Network::newConnection()
{
    if(connected)
        qDebug() << "CONNECTION IS BUSY";
    else
    {
        qDebug() << "NEW CONNECTION";
        clientSocket = new QTcpSocket();
        clientSocket = server->nextPendingConnection();

        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(clientSocket, SIGNAL(connected()), this, SLOT(onConnected()));
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

        qDebug() << clientSocket->peerAddress() << clientSocket->peerPort();
        connected = true;
    }
}

void Network::onReadyRead()
{
    qDebug() << "NEW DATA";
}

void Network::onConnected()
{
    qDebug() << "CONNECTED";
    connected = true;
}

void Network::onDisconnected()
{
    qDebug() << "DISCONNECTED";
    connected = false;
    delete clientSocket;
}

void Network::sendData(float flatTemperature, float flatHumidity, float outsideTemperature, float outsideHumidity)
{
    if(connected)
    {
        qDebug() << flatTemperature << flatHumidity << outsideTemperature << outsideHumidity;
        QByteArray arr;
        QDataStream ds(&arr, QIODevice::WriteOnly);
        ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
        ds << flatTemperature << flatHumidity << outsideTemperature << outsideHumidity;
        qDebug() << "SEND" << clientSocket->write(arr);
    }
}
