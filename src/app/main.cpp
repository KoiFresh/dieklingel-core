#include <linphone/core.h>
#include <unistd.h>

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QTimer>
#include <iostream>
#include <linphone++/linphone.hh>

#include "gpio/Gpio.hpp"
#include "kiosk/Kiosk.hpp"
#include "mqtt/Mqtt.hpp"
#include "setup/Setup.hpp"
#include "softphone/Softphone.hpp"
#include "web/WebServer.hpp"

const QString FALLBACK_DIR = "/usr/share/dieklingel-core";

int main(int argc, char *argv[]) {
    std::string resourcesDir =
        QProcessEnvironment::systemEnvironment().value("DIEKLINGEL_CORE_RESOURCES_DIR", FALLBACK_DIR).toStdString();

    auto factory = linphone::Factory::get();
    auto path = QDir::currentPath().toStdString();
    factory->setDataResourcesDir(resourcesDir);
    factory->setImageResourcesDir(resourcesDir);
    factory->setTopResourcesDir(resourcesDir);

    auto core = factory->createCore("", "", nullptr);

    auto setup = std::make_shared<Core::Setup>(argc, argv);
    setup->script("core.js")
        ->directory(".")
        ->directory("$HOME/.config")
        ->directory("$HOME")
        ->directory("/etc/dieklingel-core")
        ->directory("/usr/share/dieklingel-core");

    setup->configureable("camera", [core]() { return std::make_shared<Camera>(core); })
        ->configureable("audio", [core]() { return std::make_shared<Audio>(core); })
        ->configureable("core.mqtt", []() { return std::make_shared<Mqtt>(); })
        ->configureable("core.kiosk", [setup]() { return std::make_shared<Kiosk>(setup); })
        ->configureable("core.web", []() { return std::make_shared<WebServer>(); })
        ->configureable("core.sip", [setup, core]() { return std::make_shared<Softphone>(setup, core); })
        ->configureable("gpio", [setup]() { return std::make_shared<Gpio>(setup->engine()); });

    return setup->exec();
}
