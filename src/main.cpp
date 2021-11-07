#include <QCoreApplication>
#include <QTimer>
#include <QSound>
#include <QtConcurrent>
#include <opencv2/opencv.hpp>
#include <curl/curl.h>
#include "backend.h"
#include "socket.h"
#include "toolbox.h"
#include "ffmpegstream.h"
#include "sipclient.h"
#include "dupserver.h"
#include "dupnotification.h"
#include "system.h"
#include "sipuri.h"
#include "io.h"
#include "cryptonia.h"

/**
 * main entry of dieklingel-core application
 * @author Kai Mayer
 * @version 2.0-0
 */

int main(int argc, char *argv[])
{   
    QCoreApplication a(argc, argv);
    //dieklingel::socket::initialize(false); // false -> Protokolle und Logs werden nur über den Socket und nicht auf der Konsole ausgegeben
    //dieklingel::backend backend;
#ifdef DEBUG
    qDebug() << "[DEBUG][main.cpp, main()] \r\n\t Compield with debug log";
#endif
    dieklingel::Io::init();
    static QSettings* config = new QSettings("/etc/dieklingel/config.ini", QSettings::NativeFormat);

    static QString g_username = config->value("Ct/Username", "none").toString();
    static QString g_key = cryptonia::Normalize( config->value("Ct/Password", "none").toString(), 32);
    static int g_duoport = config->value("Dup/Port", "8080").toInt();

    static QString iv = g_key.left(16);
    static QString g_registry = cryptonia::Encrypt(g_username, g_key, iv);
    static QString g_pushsubscriberpath = config->value("Ct/Domain", "ct.dieklingel.com").toString() + config->value("Ct/Path", "/push/subscribe").toString();
    static QString g_doorsecret = config->value("Door/Secret", "").toString();

    static QString bufferImage;

    dieklingel::dup::Server *dupserver = new dieklingel::dup::Server(QHostAddress::Any, g_duoport);
    dupserver->subscribeForPushNotification(g_pushsubscriberpath, g_username, g_key);

    QString authstring = config->value("Phone/Register", "").toString();
    QStringList auths = authstring.split(" ");
    foreach(QString auth, auths)
    {
        try {
            QStringList currauth = auth.split(":");
            QString username = currauth[0];
            QString password = currauth[1];
            QString domain = currauth[2];
            int port = currauth[3].toInt();
            bool def = ((currauth.length() > 4) && (currauth[4] == "true"));

            sip::Client::Register(username, password, domain, port, def);

        } catch (QException ex) {
#ifdef DEBUG
            qDebug() << "[DEBUG][main.cpp, main()] \r\n\t There are some errors in your Phone/Register config";
#endif
        }
    }
    //sip::Client::Register("dieklingel", "kaimayer1", "192.168.178.1", 5060, true);
    //sip::Client::Register("vm-kai.mayer-dieklingel", "Kai", "dieklingel.com",6050, false);

    QObject::connect(dupserver, &dieklingel::dup::Server::onNewNotification, [dupserver](dieklingel::dup::Notification notification, bool responseRequested){
#ifdef DEBUG
        qDebug() << "[DEBUG][main.cpp, main()] \r\n\t context: " << dieklingel::ContextAsQString(notification.context());
#endif
        dieklingel::dup::Notification response = dieklingel::dup::Notification::fromQJson(dieklingel::Context::Response, QJsonObject());
        switch (notification.context()) {
            case dieklingel::Context::Register:
            {
                response = dieklingel::dup::Notification::fromQJson(dieklingel::Context::Response, dieklingel::System::readUsers());
                break;
            }
            case dieklingel::Context::EnterPasscode:
                if(dieklingel::System::hashPow(notification.dataObject()["passcode"].toString(), 2) != g_doorsecret)
                {
#ifdef DEBUG
                    qDebug() << "[DEBUG][main.cpp, main()] \r\n\t The passcode is not correct ";
#endif
                   break;
                }
            case dieklingel::Context::SecureUnlock:
            {
                dieklingel::System::execute("unlock");
                dieklingel::dup::Notification notify = dieklingel::dup::Notification::empty();
                response = dieklingel::dup::Notification::fromNotification(dieklingel::Context::Unlock, notify);
                dupserver->send(response);
                break;
            }
            case dieklingel::Context::Unlock:
                break;
            case dieklingel::Context::DeviceUpdate:
            {
                QJsonObject data = notification.dataObject();
                QJsonArray devices = dieklingel::System::readDevices();
                bool alreadyExists = false;
                int deviceCount = devices.count();
                for(int i = 0; i < deviceCount; i++)  {
                    QJsonObject currentDevice = devices[i].toObject();
                    if(currentDevice["devicename"].toString() == data["devicename"].toString())
                    {
                        devices.replace(i, data);
                        alreadyExists = true;
                        break;
                    }
                }
                if(!alreadyExists)
                {
                    devices.append(data);
                }
                dieklingel::System::writeDevices(devices);
                break;
            }
            case dieklingel::Context::Call:
            {
                QSound::play("/etc/dieklingel/Sounds/doorbell.wav");
                dieklingel::System::execute("ring");
                QList<sip::Uri*> calls;
                QList<QString> notify;
                foreach (QJsonValue value, notification.dataObject()["call"].toArray())
                {
                    sip::Uri uri(value.toString());
                    calls.append(new sip::Uri(value.toString()));
                }
                foreach (QJsonValue value, notification.dataObject()["notify"].toArray())
                {
                    notify << value.toString();
                }
                sip::Client::Call(calls);
                auto task = [notify]()
                {
                    cv::VideoCapture capture;
                    if(capture.open(0))
                    {
                        cv::Mat snapshot;
                        capture.read(snapshot);
                        capture.release();
                        bufferImage = dieklingel::Toolbox::Mat_to_b64QString(snapshot);
                    }

                    QString title = "dieklingel.com " + QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
                    QString text = "Jemand steht vor deiner Tür!";
                    QJsonArray devices = dieklingel::System::readDevices();

                    foreach(QString username, notify)
                    {
                        foreach(QJsonValue dev, devices)
                        {
                            QJsonObject device = dev.toObject();
                            if( (username == device["username"].toString() )||
                                (username.mid(0, username.indexOf("@")) == device["username"].toString()) )
                            {
                                CURL *curl;
                                CURLcode res;
                                curl = curl_easy_init();
                                if(curl)
                                {
                                    //socket::log("send push notification", "info", "backend.cpp signClicked(QJsonObject object)");
                                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
                                    curl_easy_setopt(curl, CURLOPT_URL, "https://dev.ct.dieklingel.com/push/post");
                                    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                                    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
                                    struct curl_slist *headers = NULL;
                                    headers = curl_slist_append(headers, dieklingel::Toolbox::QString_to_Char(QString("Token: ") + g_registry));
                                    headers = curl_slist_append(headers, "Content-Type: application/json");
                                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                                    QJsonObject message;
                                    message.insert("devicename", device["devicename"]);
                                    message.insert("os",device["os"]);
                                    message.insert("server",device["server"]);
                                    message.insert("sound", device["sound"]);
                                    message.insert("token", device["token"]);
                                    message.insert("username",device["username"]);
                                    message.insert("image", bufferImage);
                                    QJsonObject notify;
                                    QJsonObject alert;
                                    alert.insert("title", title);
                                    alert.insert("body", text);
                                    notify.insert("alert", alert);
                                    notify.insert("action","call");
                                    message.insert("notify", notify);
                                    QJsonDocument document(message);
                                    QString str = document.toJson(QJsonDocument::Compact);
                                    QByteArray ba = str.toLocal8Bit();
                                    const char* json = ba.data();
                                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
                                    res = curl_easy_perform(curl);
                                    curl_easy_cleanup(curl);
                                    curl = NULL;
                                    curl_slist_free_all(headers);
                                    headers = NULL;

                                    res = curl_easy_perform(curl);
                                    curl_easy_cleanup(curl);
                                    curl = NULL;
                                    curl_slist_free_all(headers);
                                    headers = NULL;
                                }
                            }
                        }
                    }
                };
                QtConcurrent::run(task);
                break;
            }
            case dieklingel::Context::Script:
            {
                QStringList args;
                foreach(QJsonValue value, notification.dataObject()["args"].toArray())
                {
                    args << value.toString();
                }
                dieklingel::System::execute(notification.dataObject()["event"].toString(), args);
                break;
            }
            default:
                dupserver->send(notification);
                break;
        }
        if(responseRequested)
        {
            dupserver->response(response);
        }
    });


    QObject::connect(sip::Client::GetInstance(), &sip::Client::SessionStateChanged, [](sip::SessionState state){
        switch (state) {
            case sip::SessionState::Waiting:
#if DEBUG
                qDebug() << "[DEBUG][main.cpp, main()] \r\n\t Session State Waiting";
#endif
                break;
            case sip::SessionState::Active:
                dieklingel::System::execute("call-start");
#if DEBUG
                qDebug() << "[DEBUG][main.cpp, main()] \r\n\t Session State Active";
#endif
                break;
            case sip::SessionState::Ended:
                dieklingel::System::execute("call-end");
#if DEBUG
                qDebug() << "[DEBUG][main.cpp, main()] \r\n\t Session State Ended";
#endif
                break;
        }
    });

    QObject::connect(dieklingel::Io::getInstance(), &dieklingel::Io::movementDetected, [dupserver](bool risingEdge){
        dieklingel::dup::Notification notification = dieklingel::dup::Notification::fromQJson(dieklingel::Context::Movement, QJsonObject());
        dupserver->send(notification);
        if(risingEdge) {
            cv::VideoCapture capture;
            if(capture.open(0))
            {
                cv::Mat snapshot;
                capture.read(snapshot);
                capture.release();
                bufferImage = dieklingel::Toolbox::Mat_to_b64QString(snapshot);
            }
        }
    });

    QSound::play("/etc/dieklingel/Sounds/boot.wav");
    dieklingel::System::execute("boot");
    return a.exec();
}
