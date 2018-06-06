#include "core.h"
#include <unistd.h>

Core::Core(QObject *parent) : QObject(parent)
{
    qDebug() << "CORE";
    thread = new QThread(this);
    network = new Network(this);
    radio = new Radio(this);
    control = new Control(29, 25, 28, 27, 24, 23, this);
    ultrasonic = new Ultrasonic(0, 2, this);
    autopitol = new Autopilot();

    autopitol->moveToThread(thread);

    connectedClient = false;
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

    modeLEDTop = 0;
    modeLEDBottom = 0;

    connectedRadio1 = false;
    connectedRadio2 = false;

    connect(radio, SIGNAL(dataChange(uint8_t, QByteArray)), this, SLOT(sendData(uint8_t, QByteArray)));
    connect(radio, SIGNAL(connectedRadio(int,bool)),this, SLOT(changeConnectedRadio(int,bool)));
    connect(network, SIGNAL(dataReceived(QString)), this, SLOT(dataRecieved(QString)));
    connect(network, SIGNAL(dataMoveReceived(QByteArray)),this, SLOT(dataMoveControl(QByteArray)));
    connect(network, SIGNAL(changeConnected(bool)), this, SLOT(changeConnectedClient(bool)));

    connect(thread, SIGNAL(finished()), autopitol, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(this, SIGNAL(setStatus(Autopilot::Status)), autopitol,SLOT(setStatus(Autopilot::Status)));
    connect(ultrasonic, SIGNAL(distanceChange(int)), autopitol, SLOT(setDistance(int)));
    connect(ultrasonic, SIGNAL(distanceChange(int)), this, SLOT(sendDataDistance(int)));

    thread->start();
}

void Core::dataRecieved(QString jsonStr)
{
    QJsonObject jsonOut = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
    foreach (QString key, jsonOut.keys()) {
        QByteArray data;
        QDataStream datastream(&data, QIODevice::WriteOnly);
        Typewrite typewrite = NONE;
        if(key == "LED_TOP")
        {
            int typeLED = 0;
            QJsonObject jsonTmpObj = jsonOut.value(key).toObject();
            if(jsonTmpObj.contains("COLOR"))
            {
                typewrite = LED;
                int red = jsonTmpObj.value("COLOR").toObject().value("RED").toDouble();
                int green = jsonTmpObj.value("COLOR").toObject().value("GREEN").toDouble();
                int blue = jsonTmpObj.value("COLOR").toObject().value("BLUE").toDouble();
                top_led.redPower = red;
                top_led.greenPower = green;
                top_led.bluePower = blue;
                datastream  << typeLED << red << green << blue;
            }

            if(jsonTmpObj.contains("MODE"))
            {
                typewrite = LED_MODE;
                modeLEDTop = jsonTmpObj.value("MODE").toInt();
                datastream  << typeLED << modeLEDTop;
            }
        }
        else if(key == "LED_BOTTOM")
        {
            int typeLED = 1;
            QJsonObject jsonTmpObj = jsonOut.value(key).toObject();
            if(jsonTmpObj.contains("COLOR"))
            {
                typewrite = LED;
                int red = jsonTmpObj.value("COLOR").toObject().value("RED").toDouble();
                int green = jsonTmpObj.value("COLOR").toObject().value("GREEN").toDouble();
                int blue = jsonTmpObj.value("COLOR").toObject().value("BLUE").toDouble();
                bottom_led.redPower = red;
                bottom_led.greenPower = green;
                bottom_led.bluePower = blue;
                datastream  << typeLED << red << green << blue;
            }

            if(jsonTmpObj.contains("MODE"))
            {
                typewrite = LED_MODE;
                modeLEDTop = jsonTmpObj.value("MODE").toInt();
                datastream  << typeLED << modeLEDTop;
            }
        }
        else if(key == "NEON")
        {
            typewrite = NEON;
            neon = jsonOut.value(key).toBool();
            datastream << neon;
        }
        radio->sendData(typewrite, data);
    }
    showData();
}

void Core::sendData(uint8_t pipe, QByteArray data)
{
    QJsonObject jsonObj;
    QDataStream datastream(&data, QIODevice::ReadOnly);
    if(pipe==1)
    {
        datastream >> flat.temperature >> flat.humidity;
        if(flat.temperature != flat.temperature)
            flat.temperature = 0;
        if(flat.humidity != flat.humidity)
            flat.humidity = 0;
        QJsonObject jsonTmp;
        jsonTmp["TEMPERATURE"] = flat.temperature;
        jsonTmp["HUMIDITY"] = flat.humidity;
        jsonObj["DHT_FLAT"] = jsonTmp;
    }
    else if(pipe==2)
    {
        datastream >> outside.temperature >> outside.humidity;
        if(outside.temperature != outside.temperature)
            outside.temperature = 0;
        if(outside.humidity != outside.humidity)
            outside.humidity = 0;
        QJsonObject jsonTmp;
        jsonTmp["TEMPERATURE"] = outside.temperature;
        jsonTmp["HUMIDITY"] = outside.humidity;
        jsonObj["DHT_OUTSIDE"] = jsonTmp;
    }
//    else
//        qDebug() << "UNKNOWN PIPE";

    QJsonDocument jsonDoc(jsonObj);
    QString strJson(jsonDoc.toJson(QJsonDocument::Compact));
    network->sendData(strJson);
    showData();
}

void Core::sendDataDistance(int distance)
{
    QJsonObject jsonObj;
    QJsonObject jsonTmp;

    jsonTmp["DISTANCE"] = distance;
    jsonObj["ULTRASONIC"] = jsonTmp;

    QJsonDocument jsonDoc(jsonObj);
    QString strJson(jsonDoc.toJson(QJsonDocument::Compact));
    network->sendData(strJson);
}

void Core::dataMoveControl(QByteArray data)
{
    int tmp_typeControl;
    QDataStream ds(&data, QIODevice::ReadOnly);
    ds >> tmp_typeControl;
    typeControl = static_cast<TypeControl>(tmp_typeControl);
    if(typeControl == TypeControl::AUTONOMUS)
    {
        connect(autopitol, SIGNAL(dataMoveReceived(int, int)), control, SLOT(move(int, int)));
        emit setStatus(Autopilot::START);
    }
    else if(typeControl == TypeControl::CONTROL)
    {
        disconnect(autopitol, SIGNAL(dataMoveReceived(int, int)), control, SLOT(move(int, int)));
        int x = 0;
        int y = 0;
        ds >> x >> y;
        emit setStatus(Autopilot::STOP);
        control->move(x, y);
    }
}

void Core::changeConnectedRadio(int pipe, bool status)
{
    QJsonObject jsonObj;
    QJsonObject jsonTmpModule1;
    QJsonObject jsonTmpModule2;
    if(pipe == 1)
        connectedRadio1 = status;
    if(!connectedRadio1)
    {
        flat.temperature = 0;
        flat.humidity = 0;
    }
    if(pipe == 2)
        connectedRadio2 = status;
    if(!connectedRadio2)
    {
        outside.temperature = 0;
        outside.humidity = 0;
    }
    jsonTmpModule1["STATUS"] = connectedRadio1;
    jsonTmpModule2["STATUS"] = connectedRadio2;
    jsonObj["MODULE1"] = jsonTmpModule1;
    jsonObj["MODULE2"] = jsonTmpModule2;
    QJsonDocument jsonDoc(jsonObj);
    QString strJson(jsonDoc.toJson(QJsonDocument::Compact));
    network->sendData(strJson);
    showData();
}

void Core::changeConnectedClient(bool connected)
{
    connectedClient = connected;
}

void Core::showData()
{
    qDebug() << "--------------------";
    qDebug() << Q_FUNC_INFO;
    qDebug() << "CLIENT CONNECTED - " << connectedClient;
    qDebug() << "MODULE 1 STATUS - " << connectedRadio1;
    qDebug() << "MODULE 2 STATUS - " << connectedRadio2;
    qDebug() << "TOP LED - " << top_led.redPower << top_led.greenPower << top_led.bluePower;
    qDebug() << "BOTTOM LED - " << bottom_led.redPower << bottom_led.greenPower << bottom_led.bluePower;
    qDebug() << "TOP LED MODE - " << modeLEDTop;
    qDebug() << "BOTTOM LED MODE - " << modeLEDBottom;
    qDebug() << "FLAT DHT - " << flat.temperature << flat.humidity;
    qDebug() << "OUTSIDE DHT - " << outside.temperature << outside.humidity;
    qDebug() << "NEON STATUS - " << neon;
}
