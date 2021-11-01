/**
 * @decprecated
 * declared as deprecated since 2021-11-01 by Kai Mayer
 **/
#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QJsonObject>
#include <QQmlEngine>
#include <QJSEngine>
#include "customwebsocketserver.h"
#include "dieklingel.h"
#include "dieklingelcontext.h"

/*
 *  socket.h
 *  stellt in verbindung mit customwebsocketserver.h den socket für die lokale DUP V1 Kommunikation bereit
 */


namespace dieklingel {

    typedef void (* receiveFunction)(QJsonObject message);
    class funcion {
    public:
        receiveFunction function;
        Context context;
    };

    class socket : public QObject
    {
        Q_OBJECT
    public:

        //explicit socket(QObject *parent = nullptr);
        static void initialize(bool terminal);
        Q_INVOKABLE static void send(Context context ,QJsonObject object);
        Q_INVOKABLE static void log(QString log, QString loglevel = "debug", QString sender = dieklingel::application::NAME);
        static customwebsocketserver* websocketserver;
        static void on(Context context, receiveFunction fu);
        static QList<funcion> receiveFunctions;

    signals:

    private:
        static bool terminal;

    private slots:
        static void textMessageReceived(QString message);
    };

    static QObject *singletonProviderForSocket(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        socket *skt = new socket();
        return skt;
    }
}

#endif // SOCKET_H
