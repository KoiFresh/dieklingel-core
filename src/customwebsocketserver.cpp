/**
 * @decprecated
 * declared as deprecated since 2021-11-01 by Kai Mayer
 **/
#include <QWebSocketServer>
#include <QWebSocket>
#include "customwebsocketserver.h"

customwebsocketserver::customwebsocketserver(QHostAddress address, int port, QObject *parent) : QObject(parent), websocketserver(new QWebSocketServer(QStringLiteral("dieklingel"), QWebSocketServer::NonSecureMode, this))
{
    if(websocketserver->listen(address , port))
    {
        qDebug() << "DUP on Port: " << port;
         connect(websocketserver, &QWebSocketServer::newConnection, this, &customwebsocketserver::onNewConnection);
    }
}

customwebsocketserver::~customwebsocketserver()
{
    websocketserver->close();
    qDeleteAll(clients.begin(), clients.end());
}

int customwebsocketserver::connectedClients()
{
    return clients.length();
}

void customwebsocketserver::onNewConnection()
{
    QWebSocket *socket = websocketserver->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &customwebsocketserver::onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &customwebsocketserver::onSocketDisconnected);
    clients << socket;
}

void customwebsocketserver::onTextMessageReceived(QString message)
{
    emit receiveTextMessage(message);
}

void customwebsocketserver::onSocketDisconnected()
{
    QWebSocket *socket = websocketserver->nextPendingConnection();
    clients.removeAll(socket);
    socket->deleteLater();
}

void customwebsocketserver::sendToEveryClient(QString message)
{
    foreach (QWebSocket* client, clients)
    {
        client->sendTextMessage(message);
    }
}
