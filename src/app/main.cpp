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
    CoreConfig conf;
    App app = App(conf);
    QGuiApplication gui(argc, argv);
    QTimer timer = QTimer();

    QObject::connect(
        &timer,
        &QTimer::timeout,
        &app, &App::iterate);
    timer.start(0);

    qmlRegisterSingletonInstance<App>("com.dieklingel", 1, 0, "App", &app);

    QQmlApplicationEngine engine;
    const QUrl url = conf.getCoreQmlEntry();
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &gui,
        [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
            {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    int exitcode = gui.exec();
    app.getCore()->stop();
    return exitcode;
}
