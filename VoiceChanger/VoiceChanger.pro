QT       += core gui
QT       += multimedia
LIBS     += -lrt -lasound -lfftw3

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Data.cpp \
    FastFourierTransform.cpp \
    FilterEcho.cpp \
    FilterPitchShift.cpp \
    FilterRobot.cpp \
    PortaudioSetup.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Constants.h \
    Data.h \
    FastFourierTransform.h \
    FilterEcho.h \
    FilterPitchShift.h \
    FilterRobot.h \
    PortaudioSetup.h \
    Structs.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
