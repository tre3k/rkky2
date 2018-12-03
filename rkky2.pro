#-------------------------------------------------
#
# Project created by QtCreator 2018-11-28T20:42:57
#
#-------------------------------------------------

QT       += core gui charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rkky2
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += function_cuda_lib/
LIBS += -L"../rkky2/function_cuda_lib/" -lfunction_cuda

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        tplot2d.cpp \
        tplot2dcase.cpp \
        tplot.cpp \
        tcomplex2d.cpp \
        tcomplex.cpp \
        qcustomplot.cpp \
    rkkyfunction.cpp

HEADERS += \
        mainwindow.h \
        tplot.h \
        tplot2d.h \
        tplot2dcase.h \
        tcomplex2d.h \
        tcomplex.h \
        qcustomplot.h \
    rkkyfunction.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
