#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include "core.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Core core;
    return a.exec();
}
