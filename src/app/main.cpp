#include <linphone/core.h>
#include <unistd.h>

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QTimer>
#include <iostream>
#include <linphone++/linphone.hh>

#include "CoreConfig.hpp"
#include "gpio/Gpio.hpp"
#include "kiosk/Kiosk.hpp"
#include "mqtt/Mqtt.hpp"
#include "setup/Setup.hpp"
#include "softphone/Softphone.hpp"
#include "web/WebServer.hpp"

int main(int argc, char *argv[]) {
    auto factory = linphone::Factory::get();
    auto path = QDir::currentPath().toStdString();
    factory->setDataResourcesDir(path);
    factory->setImageResourcesDir(path);
    factory->setTopResourcesDir(path);

    auto core = factory->createCore("", "", nullptr);

    Setup *setup = new Setup(argc, argv);
    setup->script("core.js")->directory(".");

    setup->configureable("camera", [core]() { return new Camera(core); })
        ->configureable("audio", [core]() { return new Audio(core); })
        ->configureable("core.mqtt", []() { return new Mqtt(); })
        ->configureable("core.kiosk", [setup]() { return new Kiosk(setup); })
        ->configureable("core.web", []() { return new WebServer(); })
        ->configureable("core.sip", [setup, core]() {
            return new Softphone(setup, core);
        });

    return setup->exec();
}
