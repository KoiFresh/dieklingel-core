#ifndef __KIOSK_KIOSK_HPP__
#define __KIOSK_KIOSK_HPP__

#include <QDebug>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>

#include "../setup/Configuration.hpp"
#include "../setup/Setup.hpp"
#include "InactivityDetector.hpp"

class Kiosk : public Configuration {
    Q_OBJECT
   private:
    QString _uri = "/usr/share/dieklingel-core/qml/main.qml";
    std::shared_ptr<Core::Setup> _setup;
    InactivityDetector _detector;

   public:
    Kiosk(const Kiosk &) = delete;
    Kiosk &operator=(const Kiosk &) = delete;
    Kiosk(const Kiosk &&) = delete;
    Kiosk &operator=(const Kiosk &&) = delete;
    Kiosk(std::shared_ptr<Core::Setup> setup);
    ~Kiosk() override;

    void onSetupCompleted() override;
    void print(QTextStream &log) override;

    Q_INVOKABLE void platform(QString platform);
    Q_INVOKABLE void entry(QString uri);

   signals:
    void inactivity();
};
#endif  // __KIOSK_KIOSK_HPP__
