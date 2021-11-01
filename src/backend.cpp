/**
 * @decprecated
 * declared as deprecated since 2021-11-01 by Kai Mayer
 **/
#include <QJsonObject>
#include <QJsonDocument>
#include <QtConcurrent>
#include <QCursor>
#include <opencv2/opencv.hpp>
#include <curl/curl.h>
#include <QSound>
#include "backend.h"
#include "socket.h"
#include "dieklingelcontext.h"
#include "customwebsocketserver.h"
#include "base64.h"
#include "cryptonia.h"

#ifdef Q_PROCESSOR_ARM_V7
    #include <wiringPi.h>
#endif

#define MOVEMENT_PIN 4

cv::VideoCapture dieklingel::backend::camera;
dieklingel::backend* dieklingel::backend::instance = nullptr;

// Kamera Auflösung für das PushNotification Bild
int framewidth = 920;
int frameheight = 720;

dieklingel::backend::backend(QObject *parent) : QObject(parent)
{
    // setup socket connection
    instance = this; // statischer pointer verweist auf die zuletzt erstellte instanz
    //
    connect(dieklingel::socket::websocketserver, &customwebsocketserver::receiveTextMessage, this, [](QString message) {
        QJsonDocument document = QJsonDocument::fromJson(message.toUtf8());
        QJsonObject obj;
        if(!document.isNull())
        {
            if(document.isObject())
            {
                obj = document.object();
            }
        }
    });

    // setup config file;
    config = new QSettings(QString() + dieklingel::application::BASEPATH + "config.ini", QSettings::NativeFormat);

    //setup linphone
    linphone = new customlinphone(config->value("Phone/Username").toString(),config->value("Phone/Password").toString(), config->value("Phone/Domain").toString(),config->value("Phone/Port").toInt(), this);

    connect(linphone, &customlinphone::callStarted, this, []() {
        camera.release();
        if(!QProcess::startDetached("/etc/dieklingel/scripts/on-call-start.sh"))
        {
            socket::log("Could not execute on-call-start script","warning", "backend.cpp backend(Qobeject *parent)");
        }
    });
    connect(linphone, &customlinphone::callFinished, this, []() {
        if(!QProcess::startDetached("/etc/dieklingel/scripts/on-call-end.sh"))
        {
            socket::log("Could not execute on-call-end.sh script","warning", "backend.cpp backend(Qobeject *parent)");
        }
      /*  if(!camera.isOpened()) // do not delete - even its uncomented until microwave sensor is working
        {
            camera.open(0);
            camera.set(cv::CAP_PROP_FRAME_WIDTH, framewidth);
            camera.set(cv::CAP_PROP_FRAME_HEIGHT, frameheight);
            if(!camera.isOpened())
            {
                socket::log("unable to open default camera", "warning", "backend.cpp backend(QObject *parent)");
            }
        } */
    });

    // iterate in backend thread;
    timer = new QTimer();
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &backend::iterate);
    timer->start();

    //setup camera
    framewidth = config->value("Motion/Width", "960").toInt();
    frameheight = config->value("Motion/Height", "720").toInt();


    /*camera.open(0); // do not delete - even its uncomented until microwave sensor is working
    camera.set(cv::CAP_PROP_FRAME_WIDTH, framewidth);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, frameheight);

    if(!camera.isOpened())
    {
        qDebug() << "Unable to open default camera";
        socket::log("unable to open default camera", "warning", "backend.cpp backend(QObject *parent)");
    }
 */

    socket::on(Context::EnterPasscode, [](QJsonObject object){
        QJsonObject data = object["body"].toObject()["data"].toObject();
        if(backend::instance->passcodeIsCorrect(data["passcode"].toString()))
        {
            // Passcode is corrent
            QJsonObject obj;
            socket::send(Context::Unlock, obj);
            if(!QProcess::startDetached("/etc/dieklingel/scripts/on-unlock.sh"))
            {
                socket::log("Could not execute on-unlock script(enter passcode)","warning", "backend.cpp backend(Qobeject *parent)");
            }
        }else
        {
            // Passcode is wrong
            socket::log("Wrong passcode!", "warning", "backend.cpp backend(Qobeject *parent)");
        }
    });

    socket::on(Context::Call, [](QJsonObject object){
        QJsonObject data = object["body"].toObject()["data"].toObject();
        backend::instance->signClicked(data);
        if(!QProcess::startDetached("/etc/dieklingel/scripts/on-ring.sh"))
        {
            socket::log("Could not execute on-ring script","warning", "backend.cpp backend(Qobeject *parent)");
        }
    });

    socket::on(Context::DeviceUpdate, [](QJsonObject object){
        QJsonObject body = object["body"].toObject();
        QJsonObject data = body["data"].toObject();
        QJsonArray devices = dieklingel::backend::instance->devices();

        bool alreadyExists = false;
        int index = 0;
        foreach(QJsonValue device, devices)
        {
            QJsonObject currentDevice = device.toObject();
            if(currentDevice["devicename"].toString() == data["devicename"].toString())
            {
                devices.replace(index, data);
                index++;
                alreadyExists = true;
                break;
            }
        }
        if(!alreadyExists)
        {
            devices.append(data);
        }
        dieklingel::backend::instance->setDevices(devices);
    });

    socket::on(Context::Script, [](QJsonObject object){
        QJsonObject body = object["body"].toObject();
        QJsonObject data = body["data"].toObject();
        QString application = "on-" + data["event"].toString() + ".sh";
        if(!QProcess::startDetached("/etc/dieklingel/scripts/" + application))
        {
            socket::log("Could not execute " + application + " script","warning", "backend.cpp backend(Qobeject *parent)");
        }
    });

    // Pushclient
    pushclient = new QWebSocket;
    key = cryptonia::Normalize(config->value("Ct/Password", "12345678").toString());
    ping = new QTimer();
    ping->setInterval(5000);
    QString url = "wss://" + config->value("Ct/Domain", "dieklingel.com").toString() + "/c/pushservice";
    connect(pushclient, &QWebSocket::connected, [=](){
        socket::log("connected to push service", "info", "backend.cpp connect(pushclient ...");
        QString iv = key.left(16);
        QString registry = cryptonia::Encrypt(config->value("Ct/Username", cryptonia::RandomIV(8)).toString(),key, iv);
        backend::instance->registry = registry;
        pushclient->sendTextMessage(registry);
        qDebug() << "connected";
        connect(ping, &QTimer::timeout,[=](){
            pushclient->ping();
        });
        ping->start();
    });

    connect(pushclient, &QWebSocket::textMessageReceived, [=](QString message) {
        socket::log("push message received", "info", "backend.cpp connect(pushclient ...");
        QJsonDocument doc = QJsonDocument::fromJson(cryptonia::Decrypt(message.right(message.length() - 16), key,  message.left(16)).toUtf8());
        QJsonObject obj;
        if(!doc.isNull())
        {

            obj = doc.object();
            QJsonObject body = obj["body"].toObject();
            Context ctx = QStringAsContext(body["context"].toString());
            QString iv = cryptonia::RandomIV(16);
            QString msg = iv + cryptonia::Encrypt("{}", key, iv);
            switch (ctx)
            {
                case Register:
                {
                    QFile file;
                    file.setFileName(dieklingel::application::BASEPATH + QString::fromUtf8("users.json"));
                    file.open(QIODevice::ReadOnly | QIODevice::Text);
                    QString content = file.readAll();
                    QJsonDocument data = QJsonDocument::fromJson(content.toUtf8());
                    file.close();
                    QJsonObject json;
                    QJsonObject body;
                    body.insert("context", ContextAsQString(Context::Response));
                    body.insert("data",data.array());
                    json.insert("body", body);
                    QJsonDocument jdoc(json);
                    msg = iv + cryptonia::Encrypt(jdoc.toJson(), key, iv);
                    break;
                }
                case Unlock:
                    socket::send(Context::Unlock, obj);
                    if(!QProcess::startDetached("/etc/dieklingel/scripts/on-unlock.sh"))
                    {
                        socket::log("Could not execute on-unlock script(pushserver)","warning", "backend.cpp backend(Qobeject *parent)");
                    }
                    break;
                case DeviceUpdate:
                {
                    QJsonObject data = body["data"].toObject();
                    QJsonArray devices = dieklingel::backend::instance->devices();

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
                    dieklingel::backend::instance->setDevices(devices);
                    break;
                }
                default:
                    break;
            }
            pushclient->sendTextMessage(msg);
        }
    });

    connect(pushclient, &QWebSocket::disconnected, [=]() {
        ping->stop();
        qDebug() << "pushclient disconnected";
        socket::log("disconnected from push service", "info", "backend.cpp connect(pushclient ...");
        QTimer::singleShot(5000, this, [=]() {
            pushclient->open(QUrl(url));
        });
    });

    connect(pushclient,QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), [=](QAbstractSocket::SocketError error) {
        ping->stop();
        qDebug() << "pushclient error !";
        //pushclient->close();
        /*QTimer::singleShot(5000, this, [=]() {
            pushclient->open(QUrl(url));
        });*/
    });
    pushclient->open(QUrl(url));
    qDebug() << url;

#ifdef Q_PROCESSOR_ARM_V7
    wiringPiSetup();
    pinMode(MOVEMENT_PIN, INPUT);
#endif
    // play booted sound
    QSound::play(config->value("Universal/Sound","/etc/dieklingel/sounds/boot.wav").toString());
    if(!QProcess::startDetached("/etc/dieklingel/scripts/on-boot.sh"))
    {
        qDebug() << "Could not execute on-boot script";
        socket::log("Could not execute on-boot script","warning", "backend.cpp backend(Qobeject *parent)");
    }
#ifdef Q_PROCESSOR_ARM_V7
    qDebug() << "Compiled with wiring pi";
#else
    qDebug() << "Compiled without wiring pi";
#endif
}

void dieklingel::backend::iterate()
{
#ifdef Q_PROCESSOR_ARM_V7
    bool state = digitalRead(MOVEMENT_PIN);
    if(movement > state)
    {
        socket::log("movement detected", "info", "backend.cpp iterate()");
    }
    if(state)
    {
        QJsonObject obj;
        socket::send(Context::Movement, obj);
    }
    movement = state;
#endif
}

// Führt alle notwendingen schritt aus, nachdem geklingelt wurde.
// der Parameter object sollt das Klingelschild-Object beinhalten.
// {"sip":["sip-nummer"]}
void dieklingel::backend::signClicked(QJsonObject object)
{
    QSound::play(config->value("Door/Sound","/etc/dieklingel/Sounds/doorbell.wav").toString());
    dieklingel::socket::log("sign was clicked", "info", "backend.cpp signClicked(QObject object)");
    auto task = [this](QJsonObject sign){
        QString imgbase64; // bild im als base64 string
        cv::Mat img; // bild als Matrix
        // Bild aufnehemen, welches in der Push-Benachrichtigung angezeigt wird.
        if(!camera.isOpened())
        {
            camera.open(0);
            camera.set(cv::CAP_PROP_FRAME_WIDTH, framewidth);
            camera.set(cv::CAP_PROP_FRAME_HEIGHT, frameheight);
            if(!camera.isOpened())
            {
                //socket::log("unable to open default camera", "warning", "backend.cpp backend(QObject *parent)");
            }else
            {
                camera.read(img);
            }
            camera.release();
        }
        if(!img.empty())
        {
            qInfo() << "Image converted to base64";
            std::vector<uchar> buf;
            cv::imencode(".png", img, buf);
            uchar *enc = new uchar[buf.size()];
            for(int i = 0; i < buf.size(); i++)
            {
                enc[i] = buf[i];
            }
            imgbase64 = (base64_encode(enc, buf.size())).c_str();
        }
        // Bild auf dem ct-server Hochladen.
        QString uri = "https://" + config->value("Ct/Domain","ct.dieklingel.com").toString() + "/image/" + cryptonia::RandomIV(32);
        if(!imgbase64.isEmpty())
        {
            CURL *curl;
            CURLcode res;
            curl = curl_easy_init();
            if(curl) {
              curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
              //curl_easy_setopt(curl, CURLOPT_URL, "https://kct.dieklingel.com/img");
              curl_easy_setopt(curl, CURLOPT_URL, uri.toStdString().c_str());
              curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
              curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
              struct curl_slist *headers = NULL;
              headers = curl_slist_append(headers, "Content-Type: text/plain");
              headers = curl_slist_append(headers, ("Token: " + backend::instance->registry).toLocal8Bit());
              curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
              QJsonObject upload;
              QJsonObject header;
              header.insert("token", config->value("Service/Token","none").toString());
              upload.insert("header", header);
              QJsonObject body;
              body.insert("timeout", config->value("Service/Timeout", "60000").toString());
              body.insert("data", imgbase64);
              upload.insert("body", body);
              QJsonDocument document(upload);
              QString str = document.toJson(QJsonDocument::Compact);
              QByteArray ba = str.toLocal8Bit();
              const char* json = ba.data();
              curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
              res = curl_easy_perform(curl);
            }
            curl_easy_cleanup(curl);
        }
        // Push benachrichtigung über den ct-server an Geräte senden.
        QJsonArray numbers = sign["sip"].toArray();
        QString title = "dieklingel.com " + QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
        QString text = "Jemand steht vor deiner Tür!";
        qDebug() << QJsonDocument(numbers).toJson();
        foreach(QJsonValue j, numbers)
        {
            QString number = j.toString();
            qDebug() << this->devices()[0].toObject()["username"].toString();
            foreach(QJsonValue i, this->devices())
            {
                QJsonObject device = i.toObject();
                if(number == device["username"].toString())
                {
                    QJsonObject message;
                    CURL *curl;
                    CURLcode res;
                    struct curl_slist* headers = NULL;
                    curl = curl_easy_init();
                    if(curl) {
                        //socket::log("send push notification", "info", "backend.cpp signClicked(QJsonObject object)");
                        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
                        curl_easy_setopt(curl, CURLOPT_URL, ("https://" + config->value("Ct/Domain","ct.dieklingel.com").toString() + "/pushservice").toStdString().c_str());
                        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

                        headers = curl_slist_append(headers, "Content-Type: application/json");
                        headers = curl_slist_append(headers, ("Token: " + backend::instance->registry).toLocal8Bit());

                        message.insert("devicename", device["devicename"]);
                        message.insert("os",device["os"]);
                        message.insert("server",device["server"]);
                        message.insert("sound", device["sound"]);
                        message.insert("token", device["token"]);
                        message.insert("username",device["username"]);
                        QJsonObject notify;
                        QJsonObject alert;
                        alert.insert("title", title);
                        alert.insert("body", text);
                        notify.insert("alert", alert);
                        notify.insert("image-path",uri);
                        notify.insert("action","call");
                        message.insert("notify", notify);

                        QJsonDocument document(message);
                        QString str = document.toJson(QJsonDocument::Compact);
                        QByteArray ba = str.toLocal8Bit();
                        const char* json = ba.data();
                        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
                        res = curl_easy_perform(curl);
                        qDebug() << res;
                        curl_easy_cleanup(curl);
                        curl = NULL;
                        curl_slist_free_all(headers);
                        headers = NULL;
                    }
                }
            }
        }
    };
    QtConcurrent::run(task, object);
    // Im Fritzbox modus geht ein zusätzlicher Anruf von der Klingel aus.
    if(config->value("Universal/Fritzbox", "false").toBool())
    {
        socket::log("Fritz!Box Support is enabled", "info", "backend.cpp signClicked(QJsonObject object)");
        camera.release();
        QJsonArray numbers = object["sip"].toArray();
        //linphone->call("sip:" + numbers[0].toString() + "@" + config->value("Phone/Domain", "dieklingel.com").toString() + ":" + config->value("Phone/Port").toString());
        linphone->call("sip:" + QString("vm-kai.mayer") + "@" + config->value("Phone/Domain", "dieklingel.com").toString() + ":" + config->value("Phone/Port").toString());
    }
}

// gibt an, ob der parameter 'passcode'(raw) mit dem in der Konfiguration hinterlegen doppelten hash übereinstimmt.
bool dieklingel::backend::passcodeIsCorrect(QString passcode)
{
    QByteArray firstHash = QCryptographicHash::hash(passcode.toLocal8Bit(), QCryptographicHash::Sha256).toHex();
    QString inputHash = QCryptographicHash::hash(firstHash, QCryptographicHash::Sha256).toHex();
    QString correctHash = config->value("Door/Secret", "0").toString();
    return ((correctHash == inputHash) ? true : false);
}

// speichert den QJsonArray in der Geräte-Konfigurationsdatei
void dieklingel::backend::setDevices(QJsonArray array)
{
    dieklingel::socket::log("devices are updated", "info", "backend.cpp setDevices(QJsonArray array)");
    QJsonDocument document(array);
    QFile file;
    file.setFileName(config->value("Devices/Path","/etc/dieklingel/devices.json").toString());
    file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate);
    file.write(document.toJson());
    file.close();
}

// gibt alle in der Konfiguration vorhandenen Geräte im Json Format zurück
QJsonArray dieklingel::backend::devices()
{
    QFile file;
    file.setFileName(config->value("Devices/Path","/etc/dieklingel/devices.json").toString());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());
    file.close();
    m_devices = document.array();
    return  m_devices;
}

// unbenutze funktion, wurde ursprünglich benutzt, um bewegung/veränderung auf zwei Kamera Bilder fest zustellen.
/*
bool dieklingel::backend::picturesAreDifferent(cv::Mat img1, cv::Mat img2)
{
    if(!img1.empty() && !img2.empty())
    {
        cv::cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(img1, img1, cv::Size(21, 21), 0);
        cv::cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(img2, img2, cv::Size(21, 21), 0);
        std::vector<std::vector<cv::Point>> cnts;
        cv::Mat frameDelta;
        absdiff(img1, img2, frameDelta);
        cv::Mat thresholdMat;
        threshold(frameDelta, thresholdMat, 25, 255, cv::THRESH_BINARY);
        dilate(thresholdMat, thresholdMat, cv::Mat(), cv::Point(-1,-1), 2);
        findContours(thresholdMat, cnts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        return (cnts.size() > 0);
    }
    return false;
}
*/
