#ifndef DUPSERVER_H
#define DUPSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>
#include "dieklingelcontext.h"
#include "customwebsocketserver.h"
#include "dupnotification.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace dieklingel
{
    namespace dup {

        class Server : public QObject
        {
            Q_OBJECT
        public:
            explicit Server(QHostAddress address, int port, QObject *parent = nullptr);
            ~Server();
            void subscribeForPushNotification(QString url, QString username, QString key, QString protocl = "wss");
            void response(Notification notification);
        signals:
            void onNewNotification(Notification notification, bool responseRequested = false);
        public slots:
            void send(Notification notification);
        private:
            void m_sendToEveryClient(QString message);
            QWebSocketServer *m_websocketserver;
            QList<QWebSocket*> m_clients;
            QWebSocket *m_pushclient;
            QString m_username;
            QString m_key;
            QTimer *m_ping;
        private slots:
            void m_onNewConnection();
            void m_onSocketDisconnected();
            void m_onTextMessageReceived(QString message);
            void m_onPushClientConnected();
            void m_onPushClientDisconnected();
            void m_onPushTextMessageReceived(QString message);
        };
    }
}


#endif // DUPSERVER_H
