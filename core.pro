QT += core

TEMPLATE = app
TARGET = core

HEADERS += \
    src/listeners/CoreCallback.hpp

SOURCES += \
    src/main.cpp \
    src/listeners/CoreCallback.cpp

LIBS += \
    -llinphone++

DESTDIR = bin
