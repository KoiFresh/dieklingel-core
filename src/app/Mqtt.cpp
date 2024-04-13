#include "Mqtt.hpp"

Mqtt::Mqtt(QString address) {
    this->_address = address;
    this->_mqtt = std::make_shared<mqtt::async_client>(
        address.toStdString(),
        QUuid().toString().toStdString(),
        nullptr
    );
    this->_mqtt->set_callback(*this);
}

Mqtt::~Mqtt() {}

void Mqtt::connect(QString username, QString password) {
    this->_username = username;
    this->_password = password;

    auto message = mqtt::message(
        "dieklingel/core/system/state",
        "disconnected",
        mqtt::GRANTED_QOS_2,
        true
    );
    auto options = mqtt::connect_options_builder()
                       .clean_session()
                       .user_name(username.toStdString())
                       .password(password.toStdString())
                       .will(message)
                       .finalize();

    qDebug() << "Start connecting to mqtt broker";

    QtConcurrent::run([this, options]() {
        std::shared_ptr<mqtt::token> token;
        for (;;) {
            try {
                token = this->_mqtt->connect(options);
                token->wait();
                this->_mqtt->publish(
                    "dieklingel/core/system/state",
                    "connected",
                    mqtt::GRANTED_QOS_2,
                    true
                );
                for (auto topic : this->_subscriptions) {
                    this->_mqtt->subscribe(
                        topic.toStdString(),
                        mqtt::GRANTED_QOS_2
                    );
                }
                return;
            } catch (mqtt::exception e) {
                const auto time = 10;
                qWarning() << QString(
                                  "Failed to connect to the mqtt broker %1. "
                                  "Reason: %2. "
                                  "Establish a connection will be tried again "
                                  "in %3 seconds."
                )
                                  .arg(this->_address)
                                  .arg(e.get_message().c_str())
                                  .arg(time)
                                  .toStdString()
                                  .c_str();
                QThread::sleep(time);
            }
        }
    });
}

void Mqtt::subscribe(QString topic) {
    this->_subscriptions.append(topic);
    if (this->_mqtt->is_connected()) {
        this->_mqtt->subscribe(
            topic.toStdString().c_str(),
            mqtt::GRANTED_QOS_2
        );
    }
}

void Mqtt::publish(QString topic, QString message) {
    if (!this->_mqtt->is_connected()) {
        return;
    }
    this->_mqtt->publish(topic.toStdString(), message.toStdString());
}

void Mqtt::connected(const std::string &cause) {
    qDebug() << "Successfully established a mqtt connection";
}

void Mqtt::connection_lost(const std::string &cause) {
    qDebug() << "The connection to the mqtt broker was lost";
    this->connect(this->_username, this->_password);
}

void Mqtt::message_arrived(mqtt::const_message_ptr message) {
    this->messageReceived(
        QString(message->get_topic().c_str()),
        QString(message->get_payload_str().c_str())
    );
}
