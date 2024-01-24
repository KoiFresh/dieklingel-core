#ifndef __MQTT_HPP__
#define __MQTT_HPP__

#include <mqtt/client.h>
#include <QLoggingCategory>
#include <QtConcurrent>
#include <QString>

class Mqtt : public mqtt::callback
{
private:
	QString _address;
	QString _username;
	QString _password;
	std::shared_ptr<mqtt::async_client> _mqtt;

public:
	Mqtt(QString address);
	~Mqtt();

	void connect(QString username, QString password);
	void publish(QString topic, QString message);

	void connected(const std::string &cause) override;
	void connection_lost(const std::string &cause) override;
};

#endif // __MQTT_HPP__
