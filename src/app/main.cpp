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

    auto setup = std::make_shared<Core::Setup>(argc, argv);
    setup->script("core.js")->directory(".");

    setup
        ->configureable(
            "camera",
            [core]() { return std::make_shared<Camera>(core); }
        )
        ->configureable(
            "audio",
            [core]() { return std::make_shared<Audio>(core); }
        )
        ->configureable("core.mqtt", []() { return std::make_shared<Mqtt>(); })
        ->configureable(
            "core.kiosk",
            [setup]() { return std::make_shared<Kiosk>(setup); }
        )
        ->configureable(
            "core.web",
            []() { return std::make_shared<WebServer>(); }
        )
        ->configureable("core.sip", [setup, core]() {
            return std::make_shared<Softphone>(setup, core);
        });

    return setup->exec();
}
