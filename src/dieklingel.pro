QT -= gui
QT += core
QT += multimedia
QT += network
QT += websockets
QT += quickcontrols2
QT += concurrent

CONFIG += c++11

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/include/opencv4
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio
LIBS += -lcurl
LIBS += -llinphone
LIBS += -lcryptopp
contains(DEFINES, Q_PROCESSOR_ARM_V7) {
   LIBS += -lwiringPi
}



SOURCES += \
        backend.cpp \
        base64.cpp \
        cryptonia.cpp \
        customlinphone.cpp \
        customwebsocketserver.cpp \
        main.cpp \
        socket.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/local/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    backend.h \
    base64.h \
    cryptonia.h \
    customlinphone.h \
    customwebsocketserver.h \
    dieklingel.h \
    dieklingelcontext.h \
    socket.h


