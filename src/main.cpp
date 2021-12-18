#include <QCoreApplication>
#include <QSound>
#include <QtConcurrent>
#include <opencv2/opencv.hpp>
#include <curl/curl.h>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QHostInfo>
#include "toolbox.h"
#include "sipclient.h"
#include "dupserver.h"
#include "dupnotification.h"
#include "system.h"
#include "sipuri.h"
#include "io.h"
#include "cryptonia.h"

#define MAX_TIME_MS 1000 // count until 1 sec
#define TIME_1_SEC 1000 // reach one second
#define TIME_500_MS 500
#define TIME_30_MS 30
#define TIME_10_MS 10
#define TIME_1_MS 1
#define MAX_UDP_DATAGRAM_SIZE 65535

#define BETA_VIDEO_IP "85.214.41.43"
#define BETA_VIDEO_PORT 2222

// meaning of comments:
// cbot => comment because of timer -> used to prevent a timer from being started by Kai Mayer, created because of thread problems

/**
 * main entry of dieklingel-core application
 * @author Kai Mayer
 * @version 2.2-2
 */

int main(int argc, char *argv[])
{   
    QCoreApplication a(argc, argv);
    //dieklingel::socket::initialize(false); // false -> Protokolle und Logs werden nur über den Socket und nicht auf der Konsole ausgegeben
    //dieklingel::backend backend;
#ifdef DEBUG
    qDebug() << "[DEBUG][main.cpp, main()] \r\n\t Compield with debug log";
#endif
    //cbot dieklingel::Io::init();
    static QSettings* config = new QSettings("/etc/dieklingel/config.ini", QSettings::NativeFormat);

    static QString g_username = config->value("Ct/Username", "none").toString();
    static QString g_key = cryptonia::Normalize( config->value("Ct/Password", "none").toString(), 32);
    static int g_duoport = config->value("Dup/Port", "8080").toInt();

    static QString iv = g_key.left(16);
    static QString g_registry = cryptonia::Encrypt(g_username, g_key, iv);
    static QString g_pushsubscriberpath = config->value("Ct/Domain", "ct.dieklingel.com").toString() + config->value("Ct/Subpath", "/push/subscribe").toString();
    static QString g_pushpostpath = config->value("Ct/Domain", "ct.dieklingel.com").toString() + config->value("Ct/Postpath", "/push/post").toString();
    static QString g_doorsecret = config->value("Door/Secret", "").toString();
    static QString bufferImage;
    static cv::Mat g_s_lastSnapshot;
    static cv::VideoCapture g_s_capture;

    dieklingel::dup::Server *dupserver = new dieklingel::dup::Server(QHostAddress::Any, g_duoport);
    //dupserver->subscribeForPushNotification(g_pushsubscriberpath, g_username, g_key);
    dupserver->subscribeForPushNotification("dev.ct.dieklingel.com:3381/push/subscribe", g_username, g_key, "ws");

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
            qInfo() << "[INFO][main.cpp, main()] \r\n\t There are some errors in your Phone/Register config";
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
                QJsonObject obj;
                obj.insert("Users", dieklingel::System::readUsers());
                response = dieklingel::dup::Notification::fromQJson(dieklingel::Context::Response,obj);
                break;
            }
            case dieklingel::Context::EnterPasscode:
                if(dieklingel::System::hashPow(notification.dataObject()["Passcode"].toString(), 2) != g_doorsecret)
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
                    if(currentDevice["Devicename"].toString() == data["Devicename"].toString())
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
                foreach (QJsonValue value, notification.dataObject()["Call"].toArray())
                {
                    sip::Uri uri(value.toString());
                    calls.append(new sip::Uri(value.toString()));
                }
                foreach (QJsonValue value, notification.dataObject()["Notify"].toArray())
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
                        bufferImage = dieklingel::Toolbox::s_mat_to_b64qstring(snapshot);
                    }else {
                        bufferImage = dieklingel::Toolbox::s_mat_to_b64qstring(g_s_lastSnapshot);
                    }

                    QString title = "dieklingel.com " + QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
                    QString text = "Jemand steht vor deiner Tür!";
                    QJsonArray devices = dieklingel::System::readDevices();

                    foreach(QString username, notify)
                    {
                        foreach(QJsonValue dev, devices)
                        {
                            QJsonObject device = dev.toObject();
                            if( (username == device["Username"].toString() )||
                                (username.mid(0, username.indexOf("@")) == device["Username"].toString()) )
                            {
                                CURL *curl;
                                CURLcode res;
                                curl = curl_easy_init();
                                if(curl)
                                {
                                    //socket::log("send push notification", "info", "backend.cpp signClicked(QJsonObject object)");
                                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
                                    //curl_easy_setopt(curl, CURLOPT_URL, "https://dev.ct.dieklingel.com/push/post");
                                    curl_easy_setopt(curl, CURLOPT_URL, dieklingel::Toolbox::QString_to_Char(g_pushpostpath));
                                    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                                    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
                                    struct curl_slist *headers = NULL;
                                    headers = curl_slist_append(headers, dieklingel::Toolbox::QString_to_Char(QString("Token: ") + g_registry));
                                    headers = curl_slist_append(headers, "Content-Type: application/json");
                                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                                    QJsonObject message;
                                    message.insert("Devicename", device["Devicename"]);
                                    message.insert("Os",device["Os"]);
                                    message.insert("Server",device["Server"]);
                                    message.insert("Sound", device["Sound"]);
                                    message.insert("Token", device["Token"]);
                                    message.insert("Username",device["Username"]);
                                    message.insert("Image", bufferImage);
                                    QJsonObject notify;
                                    QJsonObject alert;
                                    alert.insert("Title", title);
                                    alert.insert("Body", text);
                                    notify.insert("Alert", alert);
                                    notify.insert("Action","call");
                                    message.insert("Notify", notify);
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
                foreach(QJsonValue value, notification.dataObject()["Args"].toArray())
                {
                    args << value.toString();
                }
                dieklingel::System::execute(notification.dataObject()["Event"].toString(), args);
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

    QObject::connect(dieklingel::Io::s_instance(), &dieklingel::Io::movementDetected, [dupserver](bool risingEdge){
        dieklingel::dup::Notification notification = dieklingel::dup::Notification::fromQJson(dieklingel::Context::Movement, QJsonObject());
        dupserver->send(notification);
        if(risingEdge) {
            dieklingel::System::execute("movement");
        }
        /*if(risingEdge) {
            cv::VideoCapture capture;
            if(capture.open(0))
            {
                cv::Mat snapshot;
                capture.read(snapshot);
                capture.release();
                bufferImage = dieklingel::Toolbox::Mat_to_b64QString(snapshot);
            }
        }*/
    });

    QSound::play("/etc/dieklingel/Sounds/boot.wav");
    dieklingel::System::execute("boot");
    // running endless and call all Iterate functions
    int timeReached = 0;
    
    //g_s_capture.open(0);
    cv::Mat raw;
    //g_s_capture.read(g_s_lastSnapshot);
    //g_s_capture.release();
    raw = cv::imread("/home/pi/input_.jpeg");
    cv::resize(raw, g_s_lastSnapshot, cv::Size(200, 200));

    //QUdpSocket* socket = new QUdpSocket();
    while(true) {
        // every millisecond
        a.processEvents();
        sip::Client::Iterate();
        //every second
        if(timeReached % TIME_1_SEC == 0) 
        {
            dieklingel::Io::s_iterate();
        }
// show image   
            
            //cv::imshow("LIVE", snapshot);

//+g_s_capture.read(raw);

            /*if(false && timeReached % TIME_30_MS == 0) {
                cv::resize(raw, g_s_lastSnapshot, cv::Size(640,480));
                std::string str = std::to_string(timeReached);
                cv::rectangle(g_s_lastSnapshot, cv::Rect(40, 10, 100, 50), cv::Scalar(0,0,255), cv::FILLED);
                cv::putText(g_s_lastSnapshot, str, cv::Point(50,50),cv::FONT_HERSHEY_DUPLEX,1,cv::Scalar(0,255,0),2,false);
                QByteArray img = dieklingel::Toolbox::s_mat_to_qbytearray(g_s_lastSnapshot);
                if(img.size() < MAX_UDP_DATAGRAM_SIZE) 
                {
                    socket->writeDatagram(img, QHostAddress(BETA_VIDEO_IP), BETA_VIDEO_PORT);
                }else {
#if DEBUG
                qDebug() << "[DEBUG][main.cpp, main()] \r\n\t Image size is too large:" << img.size() << "bytes";
#endif
                }
            }*/
        QThread::msleep(TIME_1_MS);
        timeReached += TIME_1_MS;
        if(timeReached > MAX_TIME_MS) {
            timeReached = 0;
        }
    }
    return a.exec();
}
