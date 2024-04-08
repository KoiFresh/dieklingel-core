

#include "Mqtt.hpp"

Mqtt::Mqtt() {}

Mqtt::~Mqtt() {}

void Mqtt::auth(QString username, QString password) {
    this->_username = username;
    this->_password = password;
}

void Mqtt::broker(QString broker) { this->_broker = broker; }

void Mqtt::onSetupCompleted() {}

void Mqtt::print(QTextStream &log) {
    log << "🕸️  MQTT enabled:" << Qt::endl;
    log << "\t NOTE: This is only a placeholder for a planned feature, which "
           "is not implemented yet."
        << Qt::endl;
}

void Mqtt::subscribe(QString topic, QJSValue callback) {}

void Mqtt::publish(QString topic, QString message) {}
