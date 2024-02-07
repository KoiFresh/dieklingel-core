#include <iostream>
#include <QGuiApplication>
#include <QDir>
#include <QTimer>
#include <QSettings>
#include <QQmlApplicationEngine>
#include <linphone++/linphone.hh>
#include <unistd.h>

#include "App.hpp"
#include "CoreConfig.hpp"

int main(int argc, char *argv[])
{
    CoreConfig config;
    App app = App(argc, argv, config);

    return app.exec();
}
