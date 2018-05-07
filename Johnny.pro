QT += core network
QT -= gui

CONFIG += c++11

TARGET = Johnny
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

CONFIG(NOTPI):{
    message("NOT raspberry pi build.")
}else{
    message("raspberry pi build.")
    LIBS += -lrf24-bcm
    LIBS += -lwiringPi
    DEFINES += PI
}

SOURCES += main.cpp \
    core.cpp \
    network.cpp \
    radio.cpp \
    motor.cpp \
    control.cpp \
    ultrasonic.cpp \
    steppermotor.cpp \
    autopilot.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    core.h \
    network.h \
    radio.h \
    motor.h \
    control.h \
    ultrasonic.h \
    steppermotor.h \
    autopilot.h
