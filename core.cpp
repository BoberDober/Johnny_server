#include "core.h"
#include <unistd.h>

Core::Core(QObject *parent) : QObject(parent)
{
    qDebug() << "CORE";

    network = new Network();
    radio = new Radio();
    control = new Control(29, 25, 28, 27, 24, 23);
    ultrasonic = new Ultrasonic(0, 2);
    autopitol = new Autopilot();

    top_led.redPower = 0;
    top_led.greenPower = 0;
    top_led.bluePower = 0;

    bottom_led.redPower = 0;
    bottom_led.greenPower = 0;
    bottom_led.bluePower = 0;

    flat.temperature = 0.0;
    flat.humidity = 0.0;

    outside.temperature = 0.0;
    outside.humidity = 0.0;

    neon = false;

    connect(radio, SIGNAL(dataChange(uint8_t, QByteArray)), this, SLOT(sendData(uint8_t, QByteArray)));
    connect(network, SIGNAL(dataReceived(QString)), this, SLOT(dataRecieved(QString)));
    connect(network, SIGNAL(dataMoveReceived(QByteArray)),this, SLOT(dataMoveControl(QByteArray)));
    connect(autopitol, SIGNAL(dataMoveReceived(int, int)), control, SLOT(move(int, int)));
    connect(ultrasonic, SIGNAL(distanceChange(int)), autopitol, SLOT(setDistance(int)));
}

void Core::dataRecieved(QString jsonStr)
{
    QJsonObject jsonOut = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
    foreach (QString key, jsonOut.keys()) {
        QByteArray data;
        QDataStream datastream(&data, QIODevice::WriteOnly);
        Typewrite type = NONE;
        if(key == "LED_TOP")
        {
            type = LED;
            QJsonObject jsonTmpObj = jsonOut.value(key).toObject();
            int red = jsonTmpObj.value("RED").toDouble();
            int green = jsonTmpObj.value("GREEN").toDouble();
            int blue = jsonTmpObj.value("BLUE").toDouble();
            int type = 0;
            datastream  << type << red << green << blue;
        }
        else if(key == "LED_BOTTOM")
        {
            type = LED;
            QJsonObject jsonTmpObj = jsonOut.value(key).toObject();
            int red = jsonTmpObj.value("RED").toDouble();
            int green = jsonTmpObj.value("GREEN").toDouble();
            int blue = jsonTmpObj.value("BLUE").toDouble();
            int type = 1;
            datastream  << type << red << green << blue;
        }
        else if(key == "NEON")
        {
            type = NEON;
            qDebug() << jsonOut.value(key).toBool();
            datastream << jsonOut.value(key).toBool();
        }
        radio->sendData(type, data);
    }
}

void Core::sendData(uint8_t pipe, QByteArray data)
{
    QJsonObject jsonObj;
    QDataStream datastream(&data, QIODevice::ReadOnly);

    if(pipe==1)
    {
        qDebug() << "PIPE 1";
        datastream >> flat.temperature >> flat.humidity;
        QJsonObject jsonTmp;
        jsonTmp["TEMPERATURE"] = flat.temperature;
        jsonTmp["HUMIDITY"] = flat.humidity;
        jsonObj["DHT_FLAT"] = jsonTmp;
    }
    else if(pipe==2)
    {
        qDebug() << "PIPE 2";
        datastream >> outside.temperature >> outside.humidity;
        QJsonObject jsonTmp;
        jsonTmp["TEMPERATURE"] = outside.temperature;
        jsonTmp["HUMIDITY"] = outside.humidity;
        jsonObj["DHT_OUTSIDE"] = jsonTmp;
    }
    else
        qDebug() << "UNKNOWN PIPE";

    QJsonDocument jsonDoc(jsonObj);
    QString strJson(jsonDoc.toJson(QJsonDocument::Compact));
    network->sendData(strJson);
}

void Core::dataMoveControl(QByteArray data)
{
    int tmp_typeControl;
    QDataStream ds(&data, QIODevice::ReadOnly);
    ds >> tmp_typeControl;
    typeControl= (TypeControl)tmp_typeControl;
    if(typeControl == TypeControl::AUTONOMUS)
    {
        autopitol->setStatus(Status::START);
    }
    else if(typeControl == TypeControl::CONTROL)
    {
        int x = 0;
        int y = 0;
        ds >> x >> y;
        autopitol->setStatus(Status::STOP);
        control->move(x, y);
    }
}
