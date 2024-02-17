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
#include "gpio/Gpio.hpp"

int main(int argc, char *argv[])
{
    CoreConfig config;
    App app = App(argc, argv, config);

    qmlRegisterSingletonInstance<App>("com.dieklingel", 1, 0, "App", &app);
    qmlRegisterType<Input>("com.dieklingel.gpio", 1, 0, "Input");
    qmlRegisterType<Output>("com.dieklingel.gpio", 1, 0, "Output");

    return app.exec();
}
