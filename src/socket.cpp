/**
 * @decprecated
 * declared as deprecated since 2021-11-01 by Kai Mayer
 **/
#include <QJsonObject>
#include <QJsonDocument>
#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include "socket.h"
#include "dieklingel.h"
#include "customwebsocketserver.h"

//customwebsocketserver dieklingel::socket::websocketserver(QHostAddress::Any, 8090);
customwebsocketserver* dieklingel::socket::websocketserver = nullptr;
bool dieklingel::socket::terminal = true;
QList<dieklingel::funcion> dieklingel::socket::receiveFunctions;

void dieklingel::socket::initialize(bool terminal)
{
    websocketserver = new customwebsocketserver(QHostAddress::Any, 8090);
    socket::terminal = terminal;
    connect(websocketserver, &customwebsocketserver::receiveTextMessage, textMessageReceived);
}

void dieklingel::socket::send(Context context, QJsonObject object)
{
    if(websocketserver != nullptr)
    {
        QJsonObject header;
        QJsonObject body;
        if(object.contains("header"))
        {
            header = object["header"].toObject();
        }
        if(object.contains("body"))
        {
            body = object["body"].toObject();
        }
        body.insert("context", ContextAsQString(context));
        object.insert("header", header);
        object.insert("body", body);

        QString json(QJsonDocument(object).toJson());
        websocketserver->sendToEveryClient(json);
        if(terminal)
        {
            qInfo() << json;
        }
    }else
    {
        qInfo() << "socket is nullptr, maybe your forget to call 'dieklingel::socket::initialize();' this function should be called on the main method of your application, then the socket lives as long the aplliction lives";
    }
}

void dieklingel::socket::log(QString log, QString loglevel, QString sender)
{
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QJsonObject obj;
    obj.insert("header", {});
    QJsonObject body;
    body.insert("context", ContextAsQString(Log));
    QJsonObject data;
    data.insert("string", QString() + "'" + time + "'[" + sender + "](" + loglevel + "): " + log);
    body.insert("data", data);
    obj.insert("body", body);
    send(Log, obj);
}

void dieklingel::socket::textMessageReceived(QString message)
{
    QJsonDocument document = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj;
    if(!document.isNull())
    {
        if(document.isObject())
        {
            obj = document.object();
            QJsonObject body = obj["body"].toObject();
            Context ctx = QStringAsContext(body["context"].toString());
            //socket::log(message, "info", "socket.cpp textMessageReceived(QString message)");
            foreach (funcion func, receiveFunctions) {
                if(ctx == func.context)
                {
                    //´socket::log("registered " +  ContextAsQString(ctx) + "  - rec context " + ContextAsQString(func.context), "info", "socket.cpp textMessageReceived(QString message)");
                    func.function(obj);
                }
            }
        }
    }
}

void dieklingel::socket::on(Context context, receiveFunction fu)
{
    funcion rec;
    rec.function = fu;
    rec.context = context;
    receiveFunctions << rec;
}
