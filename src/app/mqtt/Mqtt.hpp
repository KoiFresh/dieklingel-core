#ifndef __MQTT_MQTTT_HPP__
#define __MQTT_MQTTT_HPP__

#include <QJSValue>
#include <QTextStream>

#include "../setup/Configuration.hpp"

class Mqtt : public Configuration {
    Q_OBJECT
   private:
    QString _username;
    QString _password;
    QString _broker;

   public:
    Mqtt();
    ~Mqtt();

    Q_INVOKABLE void auth(QString username, QString password);
    Q_INVOKABLE void broker(QString broker);

    Q_INVOKABLE void subscribe(QString topic, QJSValue callback);
    Q_INVOKABLE void publish(QString topic, QString message);

    void onSetupCompleted() override;
    void print(QTextStream &log) override;
};

#endif  // __MQTT_MQTTT_HPP__