#include <iostream>
#include <QGuiApplication>
#include <QDir>
#include <QTimer>
#include <QQmlApplicationEngine>
#include <linphone++/linphone.hh>
#include <unistd.h>

#include "App.hpp"

using namespace linphone;

int main(int argc, char *argv[])
{
    QGuiApplication gui(argc, argv);
    App app = App();
    QTimer timer = QTimer();

    QObject::connect(
        &timer,
        &QTimer::timeout,
        &app, &App::iterate);
    timer.start(500);

    QQmlApplicationEngine engine;
    const QUrl url("qml/main.qml");
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
