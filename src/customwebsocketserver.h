#ifndef CUSTOMWEBSOCKETSERVER_H
#define CUSTOMWEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>

/*
 *  customwebsocketserver.h
 *  stellt den Websocketserver, für DUP V1 bereit
 */


QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class customwebsocketserver : public QObject
{
    Q_OBJECT
public:
    explicit customwebsocketserver(QHostAddress address, int port, QObject *parent = nullptr);
    ~customwebsocketserver();
    void sendToEveryClient(QString message);
    int connectedClients();

signals:
    void receiveTextMessage(QString message);

private:
    QWebSocketServer* websocketserver;
    QList<QWebSocket*> clients;

private slots:
    void onNewConnection();
    void onSocketDisconnected();
    void onTextMessageReceived(QString message);
};

#endif // CUSTOMWEBSOCKETSERVER_H
