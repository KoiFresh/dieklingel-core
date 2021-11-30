#include <QJsonDocument>
#include <opencv2/opencv.hpp>
#include "dupserver.h"
#include "cryptonia.h"
#include "dupnotification.h"

dieklingel::dup::Server::Server(QHostAddress address, int port, QObject *parent) : QObject(parent), m_websocketserver(new QWebSocketServer(QStringLiteral("dieklingel"), QWebSocketServer::NonSecureMode, this))
{
    if(m_websocketserver->listen(address , port))
    {
        qInfo() << "[Info][dupserver.cpp, Server()] \r\n\t a local server is runnign on port: " << port;
        connect(m_websocketserver, &QWebSocketServer::newConnection, this, &Server::m_onNewConnection);
    }
}

dieklingel::dup::Server::~Server()
{
    m_websocketserver->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

/**
 * subscribe the pushnotification channel
 * @brief dieklingel::dup::Server::subscribeForPushNotification
 * @param url the url to connect to dev.ct.dieklingel.com
 * @param username to use
 * @param key to encrypt
 * @param protocol to use; defult = "wss", to use an unsecure sever, use "ws"
 */
void dieklingel::dup::Server::subscribeForPushNotification(QString url, QString username ,QString key, QString protocol)
{
    m_username = username;
    m_key = cryptonia::Normalize(key, 32);
    m_pushclient = new QWebSocket();
    QUrl qurl(protocol + "://" + url);
    m_pushclientUrl = qurl;
    connect(m_pushclient, &QWebSocket::connected, this, &Server::m_onPushClientConnected);
    connect(m_pushclient, &QWebSocket::disconnected, this, &Server::m_onPushClientDisconnected);
    connect(m_pushclient, &QWebSocket::textMessageReceived, this, &dieklingel::dup::Server::m_onPushTextMessageReceived);
    m_ping = new QTimer();
    m_ping->setInterval(5000);
    m_pushclient->open(qurl);
}

void dieklingel::dup::Server::send(dieklingel::dup::Notification notification)
{
    m_sendToEveryClient(notification.toString());
}

void dieklingel::dup::Server::publishImage(cv::Mat mat) 
{
    m_pushclient->sendBinaryMessage();
}

/**
 * will be called, when a new client connects to the server
 * @brief dieklingel::dup::Server::m_onNewConnection
 */
void dieklingel::dup::Server::m_onNewConnection()
{
    QWebSocket *socket = m_websocketserver->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &Server::m_onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &Server::m_onSocketDisconnected);
    m_clients << socket;
}

/**
 * will be called when a client sends a new text message to the server
 * @brief dieklingel::dup::Server::m_onTextMessageReceived
 * @param message from the client
 */
void dieklingel::dup::Server::m_onTextMessageReceived(QString message)
{
    Notification notification = Notification::fromString(message);
    emit onNewNotification(notification);
}

/**
 * will be called when a client disconnects from the server
 * @brief dieklingel::dup::Server::m_onSocketDisconnected
 */
void dieklingel::dup::Server::m_onSocketDisconnected()
{
    //QWebSocket *socket = m_websocketserver->nextPendingConnection();
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if(NULL != socket) {
        m_clients.removeAll(socket);
        socket->deleteLater();
    }
}

/**
 * will be called when th pushclient is finally connected
 * @brief dieklingel::dup::Server::m_onPushClientConnected
 */
void dieklingel::dup::Server::m_onPushClientConnected()
{
#if DEBUG
    qDebug() << "[DEBUG][dupserver.cpp, m_onPushClientConnected()] \r\n\t the pushclient subscribtion succesfully connected to the server";
#endif
    QString iv = m_key.left(16);
    QString registry = cryptonia::Encrypt(m_username,m_key,iv);
    m_pushclient->sendTextMessage(registry);
    connect(m_ping, &QTimer::timeout,[=](){
        m_pushclient->ping();
    });
    m_ping->start();
}

/**
 * will be called when a push client receives a new message
 * @brief dieklingel::dup::Server::m_onPushTextMessageReceived
 * @param message from the server
 */
void dieklingel::dup::Server::m_onPushTextMessageReceived(QString message)
{
    QString plain = cryptonia::Decrypt(message.right(message.length() - 16), m_key,  message.left(16));
    Notification notification = Notification::fromString(plain, true);
    emit onNewNotification(notification, true);
}

void dieklingel::dup::Server::response(Notification notification)
{
    QString iv = cryptonia::RandomIV(16);
    QString crypted = iv + cryptonia::Encrypt(notification.toString(), m_key, iv);
    m_pushclient->sendTextMessage(crypted);
}

/**
 * will be called, when the pushclient disconnects from the server
 * @brief dieklingel::dup::Server::m_onPushClientDisconnected
 */
void dieklingel::dup::Server::m_onPushClientDisconnected()
{
#if DEBUG
    qDebug() << "[DEBUG][dupserver.cpp, m_onPushClientDisconnected()] \r\n\t the pushclient subscribtion disconnected from the server";
#endif
    if(nullptr != m_ping) 
    {
        m_ping->stop();
    }
    QTimer::singleShot(5000, [=](){
        m_pushclient->open(m_pushclientUrl);
    });
}

/**
 * sends the message to every connected client
 * @brief dieklingel::dup::Server::m_sendToEveryClient
 * @param message to send
 */
void dieklingel::dup::Server::m_sendToEveryClient(QString message)
{
    foreach (QWebSocket* client, m_clients)
    {
        client->sendTextMessage(message);
    }
}
