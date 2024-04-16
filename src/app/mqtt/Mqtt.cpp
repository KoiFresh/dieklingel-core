

#include "Mqtt.hpp"

Mqtt::Mqtt() {}

Mqtt::~Mqtt() {}

void Mqtt::auth(QString username, QString password) {
    if (this->_isSetupCompleted) {
        throw std::logic_error(
            "Cannot set auth information, after the setup is completed"
        );
    }

    this->_username = username;
    this->_password = password;
}

void Mqtt::broker(QString broker) {
    if (this->_isSetupCompleted) {
        throw std::logic_error(
            "Cannot set broker information, after the setup is completed"
        );
    }

    this->_broker = broker;
}

void Mqtt::onSetupCompleted() {
    _isSetupCompleted = true;

    this->_client = std::make_shared<mqtt::async_client>(
        this->_broker.toStdString(),
        QUuid().toString().toStdString(),
        nullptr
    );
    this->_client->set_callback(*this);

    this->connect();
}

void Mqtt::print(QTextStream &log) {
    log << "🕸️  MQTT enabled:" << Qt::endl;
    log << "\t NOTE: This is only a placeholder for a planned feature, which "
           "is not implemented yet."
        << Qt::endl;
}

void Mqtt::connect() {
    auto message = mqtt::message(
        "dieklingel/core/system/state",
        "disconnected",
        mqtt::GRANTED_QOS_2,
        true
    );

    auto options = mqtt::connect_options_builder()
                       .clean_session()
                       .user_name(this->_username.toStdString())
                       .password(this->_password.toStdString())
                       .will(message)
                       .finalize();

    QtConcurrent::run([this, options]() {
        std::shared_ptr<mqtt::token> token;
        for (;;) {
            try {
                token = this->_client->connect(options);
                token->wait();

                this->_client->publish(
                    "dieklingel/core/system/state",
                    "connected",
                    mqtt::GRANTED_QOS_2,
                    true
                );
                for (auto topic : this->_subscriptions) {
                    this->_client->subscribe(
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
                                  .arg(this->_broker)
                                  .arg(e.get_message().c_str())
                                  .arg(time)
                                  .toStdString()
                                  .c_str();
                QThread::sleep(time);
            }
        }
    });
}

void Mqtt::subscribe(QList<QString> topics, QJSValue callback) {
    this->_subscriptions = topics;
    this->_callback = callback;

    if (this->_client && this->_client->is_connected()) {
        for (auto topic : topics) {
            this->_client->subscribe(topic.toStdString(), mqtt::GRANTED_QOS_2);
        }
    }
}

void Mqtt::publish(QString topic, QString message) {
    if (!this->_client || !this->_client->is_connected()) {
        return;
    }

    this->_client->publish(topic.toStdString(), message.toStdString());
}

void Mqtt::connected(const std::string &cause) {
    // TODO: handle successfull connection
}

void Mqtt::connection_lost(const std::string &cause) { this->connect(); }

void Mqtt::message_arrived(mqtt::const_message_ptr message) {
    QString topic = message->get_topic().c_str();
    QString payload = message->get_payload().c_str();

    QList<QJSValue> arguments = {topic, payload};
    this->_callback.call(arguments);
}
