#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QSettings>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QPoint>
#include <QtWebSockets>
#include <opencv2/opencv.hpp>
#include "dieklingel.h"
#include "customlinphone.h"

namespace dieklingel {
    class backend : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QJsonArray devices READ devices WRITE setDevices NOTIFY devicesChanged)

    public:
        explicit backend(QObject *parent = nullptr);
        Q_INVOKABLE void signClicked(QJsonObject object);
        Q_INVOKABLE bool passcodeIsCorrect(QString passcode);
        void setDevices(QJsonArray array);
        QJsonArray devices();
        static backend* instance;
        QString registry;

    signals:
        void devicesChanged();
        void messageReceived(QJsonObject object);

    private:
        bool movement;
        static cv::VideoCapture camera;
        QString key;
        //int current_ms_afterframe = 0;
        QSettings* config;
        QTimer* timer;
        QTimer* ping;
        customlinphone* linphone;
        void iterate();
        QJsonArray m_devices;
        //bool picturesAreDifferent(cv::Mat img1, cv::Mat img2);
        QWebSocket* pushclient;

    private slots:

    };
}
#endif // BACKEND_H
