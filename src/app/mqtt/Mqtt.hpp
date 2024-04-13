#ifndef __MQTT_MQTTT_HPP__
#define __MQTT_MQTTT_HPP__

#include <mqtt/client.h>

#include <QJSValue>
#include <QTextStream>
#include <QtConcurrent>

#include "../setup/Configuration.hpp"

class Mqtt : public Configuration, public mqtt::callback {
    Q_OBJECT
   private:
    QString _username;
    QString _password;
    QString _broker;
    std::shared_ptr<mqtt::async_client> _client;
    bool _isSetupCompleted = false;
    QList<QString> _subscriptions;
    QJSValue _callback;

   public:
    Mqtt();
    ~Mqtt();

    Q_INVOKABLE void auth(QString username, QString password);
    Q_INVOKABLE void broker(QString broker);

    Q_INVOKABLE void subscribe(QList<QString> topic, QJSValue callback);
    Q_INVOKABLE void publish(QString topic, QString message);

    void onSetupCompleted() override;
    void print(QTextStream &log) override;

    void connect();
    void connected(const std::string &cause) override;
    void connection_lost(const std::string &cause) override;
    void message_arrived(mqtt::const_message_ptr message) override;
};

#endif  // __MQTT_MQTTT_HPP__