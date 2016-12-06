#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T10:11:43
#
#-------------------------------------------------

QT       += core gui
QT += xml\
     serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = DeviceSimulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    define.cpp \
    functionitemwidget.cpp \
    controlItem/controlcmd.cpp \
    controlItem/controlnumber.cpp \
    controlItem/controlenumberate.cpp \
    controlItem/controlalarm.cpp \
    controlItem/controldate.cpp \
    controlItem/controltime.cpp \
    controlpanel.cpp \
    controlItem/controldatetime.cpp \
    detectxml.cpp \
    serialport.cpp \
    UserUartLink.cpp \
    about.cpp \
    controlItem/selectpanel.cpp

HEADERS  += mainwindow.h \
    define.h \
    functionitemwidget.h \
    controlItem/controlcmd.h \
    controlItem/controlnumber.h \
    controlItem/controlenumberate.h \
    controlItem/controlalarm.h \
    controlItem/controldate.h \
    controlItem/controltime.h \
    controlpanel.h \
    controlItem/controldatetime.h \
    detectxml.h \
    serialport.h \
    UserUartLink.h \
    about.h \
    controlItem/selectpanel.h

FORMS    += mainwindow.ui \
    controlItem/controlcmd.ui \
    controlItem/controlnumber.ui \
    controlItem/controlenumberate.ui \
    controlItem/controlalarm.ui \
    controlItem/controldate.ui \
    controlItem/controltime.ui \
    controlItem/controldatetime.ui \
    about.ui \
    controlItem/selectpanel.ui

RESOURCES += \
    resource.qrc
